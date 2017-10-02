#if !(defined MPL_ENVIRONMENT_HPP)

#define MPL_ENVIRONMENT_HPP

#include <string>
#include <memory>
#include <vector>
#include <mpi.h>

namespace mpl {

enum class threading_modes { single = MPI_THREAD_SINGLE,
    funneled = MPI_THREAD_FUNNELED,
    serialized = MPI_THREAD_SERIALIZED,
    multiple = MPI_THREAD_MULTIPLE };

namespace environment {

    namespace detail {

        class env {
            class initializer {
            public:
                initializer() {
                    int thread_mode;
                    MPI_Init_thread(0, 0, MPI_THREAD_MULTIPLE, &thread_mode);
                }
                ~initializer() {
                    MPI_Finalize();
                }
            };

            initializer init;
            mpl::communicator comm_world_, comm_self_;

        public:
            env()
                : init()
                , comm_world_(MPI_COMM_WORLD)
                , comm_self_(MPI_COMM_SELF) {
                int size;
                MPI_Comm_size(MPI_COMM_WORLD, &size);
            }
            env(const env&) = delete;
            env& operator=(const env&) = delete;
            int tag_up() const {
                void* p;
                int flag;
                MPI_Comm_get_attr(MPI_COMM_WORLD, MPI_TAG_UB, &p, &flag);
                return *reinterpret_cast<int*>(p);
            }
            threading_modes threading_mode() const {
                int provided;
                MPI_Query_thread(&provided);
                switch (provided) {
                case MPI_THREAD_SINGLE:
                    return threading_modes::single;
                case MPI_THREAD_FUNNELED:
                    return threading_modes::funneled;
                case MPI_THREAD_SERIALIZED:
                    return threading_modes::serialized;
                case MPI_THREAD_MULTIPLE:
                    return threading_modes::multiple;
                }
                return threading_modes::single; // make compiler happy
            }
            bool is_thread_main() const {
                int res;
                MPI_Is_thread_main(&res);
                return static_cast<bool>(res);
            }
            bool wtime_is_global() const {
                void* p;
                int flag;
                MPI_Comm_get_attr(MPI_COMM_WORLD, MPI_WTIME_IS_GLOBAL, &p, &flag);
                return *reinterpret_cast<int*>(p);
            }
            const communicator& comm_world() const {
                return comm_world_;
            }
            const communicator& comm_self() const {
                return comm_self_;
            }
            std::string processor_name() const {
                char name[MPI_MAX_PROCESSOR_NAME];
                int len;
                MPI_Get_processor_name(name, &len);
                return std::string(name);
            }
            double wtime() const {
                return MPI_Wtime();
            }
            double wtick() const {
                return MPI_Wtick();
            }
            void buffer_attach(void* buff, int size) const {
                MPI_Buffer_attach(buff, size);
            }
            std::pair<void*, int> buffer_detach() const {
                void* buff;
                int size;
                MPI_Buffer_detach(&buff, &size);
                return std::make_pair(buff, size);
            }
        };

        //----------------------------------------------------------------

        const env& get_env() {
            static env the_env;
            return the_env;
        }
    }

    //------------------------------------------------------------------

    inline int tag_up() {
        return detail::get_env().tag_up();
    }

    constexpr int any_tag() {
        return MPI_ANY_TAG;
    }

    constexpr int any_source() {
        return MPI_ANY_SOURCE;
    }

    constexpr int proc_null() {
        return MPI_PROC_NULL;
    }

    constexpr int undefined() {
        return MPI_UNDEFINED;
    }

    constexpr int root() {
        return MPI_ROOT;
    }

    constexpr void* absolute() {
        return reinterpret_cast<void*>(MPI_BOTTOM);
    }

    constexpr int bsend_overheadroot() {
        return MPI_BSEND_OVERHEAD;
    }

    inline threading_modes threading_mode() {
        return detail::get_env().threading_mode();
    }

    inline bool is_thread_main() {
        return detail::get_env().is_thread_main();
    }

    inline bool wtime_is_global() {
        return detail::get_env().wtime_is_global();
    }

    inline const communicator& comm_world() {
        return detail::get_env().comm_world();
    }

    inline const communicator& comm_self() {
        return detail::get_env().comm_self();
    }

    inline std::string processor_name() {
        return detail::get_env().processor_name();
    }

    inline double wtime() {
        return detail::get_env().wtime();
    }

    inline double wtick() {
        return detail::get_env().wtick();
    }

    inline void buffer_attach(void* buff, int size) {
        return detail::get_env().buffer_attach(buff, size);
    }

    inline std::pair<void*, int> buffer_detach() {
        return detail::get_env().buffer_detach();
    }
}

//--------------------------------------------------------------------

template <typename A = std::allocator<char>>
class bsend_buffer {
    int size;
    A alloc;
    char* buff;

public:
    bsend_buffer(int size)
        : size(size)
        , alloc()
        , buff(alloc.allocate(size)) {
        environment::buffer_attach(buff, size);
    }
    bsend_buffer(int size, A alloc)
        : size(size)
        , alloc(alloc)
        , buff(alloc.allocate(size)) {
        environment::buffer_attach(buff, size);
    }
    ~bsend_buffer() {
        environment::buffer_detach();
        alloc.deallocate(buff, size);
    }
};
}

#endif
