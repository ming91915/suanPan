#if !(defined MPL_REQUEST_HPP)

#define MPL_REQUEST_HPP

#include <mpi.h>
#include <utility>
#include <vector>

namespace mpl {

namespace detail {

    template <typename T> class request;

    template <typename T> class request_pool;

    class irequest {
        MPI_Request req = MPI_REQUEST_NULL;

    public:
        irequest(MPI_Request req)
            : req(req) {}
        friend class request<irequest>;
        friend class request_pool<irequest>;
    };

    class prequest {
        MPI_Request req = MPI_REQUEST_NULL;

    public:
        prequest(MPI_Request req)
            : req(req) {}
        friend class request<prequest>;
        friend class request_pool<prequest>;
    };

    //------------------------------------------------------------------

    template <typename T> class request {
    protected:
        MPI_Request req;

    public:
        request() = delete;
        request(const request&) = delete;
        request(const T& other)
            : req(other.req) {}
        request(T&& other)
            : req(other.req) {
            other.req = MPI_REQUEST_NULL;
        }
        ~request() {
            if(req != MPI_REQUEST_NULL) MPI_Request_free(&req);
        }
        void operator=(const request&) = delete;
        request& operator=(request&& other) {
            if(this != &other) {
                if(req != MPI_REQUEST_NULL) MPI_Request_free(&req);
                req = other.req;
                other.req = MPI_REQUEST_NULL;
            }
            return *this;
        }
        void cancel() { MPI_Cancel(&req); }
        std::pair<bool, status> test() {
            int result;
            status s;
            MPI_Test(&req, &result, reinterpret_cast<MPI_Status*>(&s));
            return std::make_pair(static_cast<bool>(result), s);
        }
        status wait() {
            status s;
            MPI_Wait(&req, reinterpret_cast<MPI_Status*>(&s));
            return s;
        }
        std::pair<bool, status> get_status() {
            int result;
            status s;
            MPI_Request_get_status(req, &result, reinterpret_cast<MPI_Status*>(&s));
            return std::make_pair(static_cast<bool>(result), s);
        }
    };

    //------------------------------------------------------------------

    template <typename T> class request_pool {
    protected:
        std::vector<MPI_Request> reqs;
        std::vector<status> stats;

    public:
        typedef std::vector<MPI_Request>::size_type size_type;
        request_pool() {}
        request_pool(const request_pool&) = delete;
        request_pool(request_pool&& other)
            : reqs(std::move(other.reqs))
            , stats(std::move(other.stats)) {}
        ~request_pool() {
            for(std::vector<MPI_Request>::iterator i(reqs.begin()), i_end(reqs.end()); i != i_end; ++i)
                if((*i) != MPI_REQUEST_NULL) MPI_Request_free(&(*i));
        }
        void operator=(const request_pool&) = delete;
        request_pool& operator=(request_pool&& other) {
            if(this != &other) {
                for(std::vector<MPI_Request>::iterator i(reqs.begin()), i_end(reqs.end()); i != i_end; ++i)
                    if((*i) != MPI_REQUEST_NULL) MPI_Request_free(&(*i));
                reqs = std::move(other.reqs);
                stats = std::move(other.stats);
            }
            return *this;
        }
        size_type size() const { return reqs.size(); }
        bool empty() const { return reqs.empty(); }
        const status& get_status(size_type i) const { return stats[i]; }
        void cancel(size_type i) { MPI_Cancel(&reqs[i]); }
        void cancelall() {
            for(size_type i = 0; i < reqs.size(); ++i) cancel(i);
        }
        void push(const T& other) {
            reqs.push_back(other.req);
            stats.push_back(status());
        }
        std::pair<bool, size_type> waitany() {
            int index;
            status s;
            MPI_Waitany(size(), &reqs[0], &index, reinterpret_cast<MPI_Status*>(&s));
            if(index != MPI_UNDEFINED) {
                stats[index] = s;
                return std::make_pair(true, static_cast<size_type>(index));
            }
            return std::make_pair(false, size());
        }
        std::pair<bool, size_type> testany() {
            int index, flag;
            status s;
            MPI_Testany(size(), &reqs[0], &index, &flag, reinterpret_cast<MPI_Status*>(&s));
            if(flag and index != MPI_UNDEFINED) {
                stats[index] = s;
                return std::make_pair(true, static_cast<size_type>(index));
            }
            return std::make_pair(static_cast<bool>(flag), size());
        }
        void waitall() { MPI_Waitall(size(), &reqs[0], reinterpret_cast<MPI_Status*>(&stats[0])); }
        bool testall() {
            int flag;
            MPI_Testall(size(), &reqs[0], &flag, reinterpret_cast<MPI_Status*>(&stats[0]));
            return flag;
        }
        template <typename I> I waitsome(I iter) {
            flat_memory_out<int, I> indices(size(), iter);
            int count;
            MPI_Waitsome(size(), &reqs[0], &count, indices.data(), reinterpret_cast<MPI_Status*>(&stats[0]));
            if(count != MPI_UNDEFINED) return indices.copy_back(count);
            return iter;
        }
        template <typename I> I testsome(I iter) {
            flat_memory_out<int, I> indices(size(), iter);
            int count;
            MPI_Testsome(size(), &reqs[0], &count, indices.data(), reinterpret_cast<MPI_Status*>(&stats[0]));
            if(count != MPI_UNDEFINED) return indices.copy_back(count);
            return iter;
        }
    };
}

//--------------------------------------------------------------------

class irequest : public detail::request<detail::irequest> {
    typedef detail::request<detail::irequest> base;
    using base::req;

public:
    irequest(const detail::irequest& r)
        : base(r) {}
    irequest(const irequest&) = delete;
    irequest(irequest&& r)
        : base(std::move(r.req)) {}
    void operator=(const irequest&) = delete;
    irequest& operator=(irequest&& other) {
        if(this != &other) base::operator=(std::move(other));
        return *this;
    }
};

//--------------------------------------------------------------------

class irequest_pool : public detail::request_pool<detail::irequest> {
    typedef detail::request_pool<detail::irequest> base;

public:
    irequest_pool() {}
    irequest_pool(const irequest_pool&) = delete;
    irequest_pool(irequest_pool&& r)
        : base(std::move(r)) {}
    void operator=(const irequest_pool&) = delete;
    irequest_pool& operator=(irequest_pool&& other) {
        if(this != &other) base::operator=(std::move(other));
        return *this;
    }
};

//--------------------------------------------------------------------

class prequest : public detail::request<detail::prequest> {
    typedef detail::request<detail::prequest> base;
    using base::req;

public:
    prequest(const detail::prequest& r)
        : base(r) {}
    prequest(const prequest&) = delete;
    prequest(prequest&& r)
        : base(std::move(r.req)) {}
    void operator=(const prequest&) = delete;
    prequest& operator=(prequest&& other) {
        if(this != &other) base::operator=(std::move(other));
        return *this;
    }
    void start() { MPI_Start(&req); }
};

//--------------------------------------------------------------------

class prequest_pool : public detail::request_pool<detail::prequest> {
    typedef detail::request_pool<detail::prequest> base;
    using base::reqs;

public:
    prequest_pool() {}
    void operator=(const prequest_pool&) = delete;
    prequest_pool& operator=(prequest_pool&& other) {
        if(this != &other) base::operator=(std::move(other));
        return *this;
    }
    prequest_pool(prequest_pool&& r)
        : base(std::move(r)) {}
    void startall() { MPI_Startall(size(), &reqs[0]); }
};
}

#endif
