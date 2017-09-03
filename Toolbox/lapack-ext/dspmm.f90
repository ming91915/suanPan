    ! Purpose:  To compute alpha*op(A,B)+beta*C, op(A,B) could be either
    !           A*B,
    !           B*A,
    !           A*B**T,
    !           B**T*A.
    !
    ! \param[in] SIDE is CHARACTER
    !            Select which problem to compute:
    !            'L'    C=alpha*B*A+beta*C,
    !            'R'    C=alpha*A*B+beta*C.
    ! \param[in] UPLO is CHARACTER
    !            Select which part of A is stored:
    !            'U'    Upper Triangle,
    !            'L'    Lower Triangle.
    ! \param[in] TRAN is CHARACTER
    !            Select if B is transverse:
    !            'N'    No transverse,
    !            'T'    Transverse.
    ! \param[in] M is INTEGER
    !            The size of square matrix A.
    ! \param[in] N is INTEGER
    !            Another dimension of matrix B.
    !            For SIDE='L', B=>(N,M),
    !            For SIDE='R', B=>(M,N),
    ! \param[in] ALPHA is DOUBLEPRECISION
    !            The factor.
    ! \param[in] A is DOUBLEPRECISION(*) array of DIMENSION ((M+1)*M/2)
    ! \param[in] B is DOUBLEPRECISION(*,*) array of DIMENSION (M,N) or (N,M)
    ! \param[in] LDB is INTEGER
    !            The leading dimension of matrix B, should be at least max(1,M) or max(1,N).
    ! \param[in] BETA is DOUBLEPRECISION
    !            The factor.
    ! \param[in/out] C is DOUBLEPRECISION(*,*) array of DIMENSION (M,N) or (N,M)
    ! \param[in] LDC is INTEGER
    !            The leading dimension of matrix C, should be at least max(1,M) or max(1,N).
    !
    SUBROUTINE DSPMM(SIDE,UPLO,TRAN,M,N,A,ALPHA,B,LDB,BETA,C,LDC)

    !...INPUR ARGUMENTS...
    CHARACTER SIDE,UPLO,TRAN
    INTEGER M,N,LDB,LDC
    DOUBLEPRECISION ALPHA,BETA,A(*),B(LDB,*),C(LDC,*)

    !...TEMP VARIABLES...
    INTEGER I,J,K,X,Y,Z,DIMA,DIMB,PTYPE
    DOUBLEPRECISION TEMPA,TEMPB

    !...TWO CONSTANTS...
    DOUBLEPRECISION ZERO,ONE
    PARAMETER (ZERO=0.0D+0,ONE=1.0D+0)

    !...SWITCH TO PROPER DIMENSION...
    IF(LSAME(SIDE,'R'))THEN
        DIMA=M
        DIMB=N
    ELSE
        DIMA=N
        DIMB=M
    ENDIF

    !...QUICK RETURN...
    IF(ALPHA.EQ.ZERO)THEN
        IF(BETA.EQ.ZERO)THEN
            DO J=1,DIMB
                DO I=1,DIMA
                    C(I,J)=ZERO
                ENDDO
            ENDDO
        ELSEIF(BETA.NE.ONE)THEN
            DO J=1,DIMB
                DO I=1,DIMA
                    C(I,J)=BETA*C(I,J)
                ENDDO
            ENDDO
        ENDIF
        RETURN
    ENDIF

    !...CHECK beta*C FIRST...
    IF(BETA.EQ.ZERO)THEN
        DO J=1,DIMB
            DO I=1,DIMA
                C(I,J)=ZERO
            ENDDO
        ENDDO
    ELSEIF(BETA.NE.ONE)THEN
        DO J=1,DIMB
            DO I=1,DIMA
                C(I,J)=BETA*C(I,J)
            ENDDO
        ENDDO
    ENDIF

    !...ASSIGN PROBLEM TYPE ACCORDING TO GIVEN FLAGS...
    PTYPE=0000
    IF(LSAME(SIDE,'R'))PTYPE=PTYPE+1000
    IF(LSAME(UPLO,'L'))PTYPE=PTYPE+100
    IF(LSAME(TRAN,'N'))PTYPE=PTYPE+10
    IF(ALPHA.EQ.ONE)PTYPE=PTYPE+1

    !... NOW DO MULTIPLICATION FOR DIFFERENT CASES...
    IF(PTYPE.EQ.1111)THEN
        DO J=1,DIMB
            DO I=1,DIMA
                TEMPA=ZERO
                X=I
                Y=1
                DO K=M-1,M-I+1,-1
                    IF(B(Y,J).NE.ZERO)TEMPA=TEMPA+A(X)*B(Y,J)
                    X=X+K
                    Y=Y+1
                ENDDO
                DO Z=X,M-I+X
                    IF(B(Y,J).NE.ZERO)TEMPA=TEMPA+A(Z)*B(Y,J)
                    Y=Y+1
                ENDDO
                C(I,J)=C(I,J)+TEMPA
            ENDDO
        ENDDO
        RETURN
    ENDIF

    IF(PTYPE.EQ.1101)THEN
        DO J=1,DIMB
            DO I=1,DIMA
                TEMPA=ZERO
                X=I
                Y=1
                DO K=M-1,M-I+1,-1
                    IF(B(J,Y).NE.ZERO)TEMPA=TEMPA+A(X)*B(J,Y)
                    X=X+K
                    Y=Y+1
                ENDDO
                DO Z=X,M-I+X
                    IF(B(J,Y).NE.ZERO)TEMPA=TEMPA+A(Z)*B(J,Y)
                    Y=Y+1
                ENDDO
                C(I,J)=C(I,J)+TEMPA
            ENDDO
        ENDDO
        RETURN
    ENDIF

    IF(PTYPE.EQ.1110)THEN
        DO J=1,DIMB
            DO I=1,DIMA
                TEMPA=ZERO
                X=I
                Y=1
                DO K=M-1,M-I+1,-1
                    IF(B(Y,J).NE.ZERO)TEMPA=TEMPA+A(X)*B(Y,J)
                    X=X+K
                    Y=Y+1
                ENDDO
                DO Z=X,M-I+X
                    IF(B(Y,J).NE.ZERO)TEMPA=TEMPA+A(Z)*B(Y,J)
                    Y=Y+1
                ENDDO
                C(I,J)=C(I,J)+TEMPA*ALPHA
            ENDDO
        ENDDO
        RETURN
    ENDIF

    IF(PTYPE.EQ.1100)THEN
        DO J=1,DIMB
            DO I=1,DIMA
                TEMPA=ZERO
                X=I
                Y=1
                DO K=M-1,M-I+1,-1
                    IF(B(J,Y).NE.ZERO)TEMPA=TEMPA+A(X)*B(J,Y)
                    X=X+K
                    Y=Y+1
                ENDDO
                DO Z=X,M-I+X
                    IF(B(J,Y).NE.ZERO)TEMPA=TEMPA+A(Z)*B(J,Y)
                    Y=Y+1
                ENDDO
                C(I,J)=C(I,J)+TEMPA*ALPHA
            ENDDO
        ENDDO
        RETURN
    ENDIF

    IF(PTYPE.EQ.1011)THEN
        DO J=1,DIMB
            DO I=1,DIMA
                TEMPA=ZERO
                TEMPB=(I-1)*I/2
                Y=1
                DO X=TEMPB+1,TEMPB+I
                    IF(B(Y,J).NE.ZERO)TEMPA=TEMPA+A(X)*B(Y,J)
                    Y=Y+1
                ENDDO
                X=X-1
                DO Z=I,M-1
                    X=X+Z
                    IF(B(Y,J).NE.ZERO)TEMPA=TEMPA+A(X)*B(Y,J)
                    Y=Y+1
                ENDDO
                C(I,J)=C(I,J)+TEMPA
            ENDDO
        ENDDO
        RETURN
    ENDIF

    IF(PTYPE.EQ.1001)THEN
        DO J=1,DIMB
            DO I=1,DIMA
                TEMPA=ZERO
                TEMPB=(I-1)*I/2
                Y=1
                DO X=TEMPB+1,TEMPB+I
                    IF(B(J,Y).NE.ZERO)TEMPA=TEMPA+A(X)*B(J,Y)
                    Y=Y+1
                ENDDO
                X=X-1
                DO Z=I,M-1
                    X=X+Z
                    IF(B(J,Y).NE.ZERO)TEMPA=TEMPA+A(X)*B(J,Y)
                    Y=Y+1
                ENDDO
                C(I,J)=C(I,J)+TEMPA
            ENDDO
        ENDDO
        RETURN
    ENDIF

    IF(PTYPE.EQ.1010)THEN
        DO J=1,DIMB
            DO I=1,DIMA
                TEMPA=ZERO
                TEMPB=(I-1)*I/2
                Y=1
                DO X=TEMPB+1,TEMPB+I
                    IF(B(Y,J).NE.ZERO)TEMPA=TEMPA+A(X)*B(Y,J)
                    Y=Y+1
                ENDDO
                X=X-1
                DO Z=I,M-1
                    X=X+Z
                    IF(B(Y,J).NE.ZERO)TEMPA=TEMPA+A(X)*B(Y,J)
                    Y=Y+1
                ENDDO
                C(I,J)=C(I,J)+TEMPA*ALPHA
            ENDDO
        ENDDO
        RETURN
    ENDIF

    IF(PTYPE.EQ.1000)THEN
        DO J=1,DIMB
            DO I=1,DIMA
                TEMPA=ZERO
                TEMPB=(I-1)*I/2
                Y=1
                DO X=TEMPB+1,TEMPB+I
                    IF(B(Y,J).NE.ZERO)TEMPA=TEMPA+A(X)*B(Y,J)
                    Y=Y+1
                ENDDO
                X=X-1
                DO Z=I,M-1
                    X=X+Z
                    IF(B(Y,J).NE.ZERO)TEMPA=TEMPA+A(X)*B(Y,J)
                    Y=Y+1
                ENDDO
                C(I,J)=C(I,J)+TEMPA*ALPHA
            ENDDO
        ENDDO
        RETURN
    ENDIF

    IF(PTYPE.EQ.0111)THEN
        DO J=1,DIMB
            DO I=1,DIMA
                TEMPA=ZERO
                X=J
                Y=1
                DO K=M-1,M-J+1,-1
                    IF(B(I,Y).NE.ZERO)TEMPA=TEMPA+A(X)*B(I,Y)
                    X=X+K
                    Y=Y+1
                ENDDO
                DO Z=X,M-J+X
                    IF(B(I,Y).NE.ZERO)TEMPA=TEMPA+A(Z)*B(I,Y)
                    Y=Y+1
                ENDDO
                C(I,J)=C(I,J)+TEMPA
            ENDDO
        ENDDO
        RETURN
    ENDIF

    IF(PTYPE.EQ.0101)THEN
        DO J=1,DIMB
            DO I=1,DIMA
                TEMPA=ZERO
                X=J
                Y=1
                DO K=M-1,M-J+1,-1
                    IF(B(Y,I).NE.ZERO)TEMPA=TEMPA+A(X)*B(Y,I)
                    X=X+K
                    Y=Y+1
                ENDDO
                DO Z=X,M-J+X
                    IF(B(Y,I).NE.ZERO)TEMPA=TEMPA+A(Z)*B(Y,I)
                    Y=Y+1
                ENDDO
                C(I,J)=C(I,J)+TEMPA
            ENDDO
        ENDDO
        RETURN
    ENDIF

    IF(PTYPE.EQ.0110)THEN
        DO J=1,DIMB
            DO I=1,DIMA
                TEMPA=ZERO
                X=J
                Y=1
                DO K=M-1,M-J+1,-1
                    IF(B(I,Y).NE.ZERO)TEMPA=TEMPA+A(X)*B(I,Y)
                    X=X+K
                    Y=Y+1
                ENDDO
                DO Z=X,M-J+X
                    IF(B(I,Y).NE.ZERO)TEMPA=TEMPA+A(Z)*B(I,Y)
                    Y=Y+1
                ENDDO
                C(I,J)=C(I,J)+TEMPA*ALPHA
            ENDDO
        ENDDO
        RETURN
    ENDIF

    IF(PTYPE.EQ.0100)THEN
        DO J=1,DIMB
            DO I=1,DIMA
                TEMPA=ZERO
                X=J
                Y=1
                DO K=M-1,M-J+1,-1
                    IF(B(Y,I).NE.ZERO)TEMPA=TEMPA+A(X)*B(Y,I)
                    X=X+K
                    Y=Y+1
                ENDDO
                DO Z=X,M-J+X
                    IF(B(Y,I).NE.ZERO)TEMPA=TEMPA+A(Z)*B(Y,I)
                    Y=Y+1
                ENDDO
                C(I,J)=C(I,J)+TEMPA*ALPHA
            ENDDO
        ENDDO
        RETURN
    ENDIF

    IF(PTYPE.EQ.0011)THEN
        DO J=1,DIMB
            DO I=1,DIMA
                TEMPA=ZERO
                TEMPB=(J-1)*J/2
                Y=1
                DO X=TEMPB+1,TEMPB+J
                    IF(B(I,Y).NE.ZERO)TEMPA=TEMPA+A(X)*B(I,Y)
                    Y=Y+1
                ENDDO
                X=X-1
                DO Z=J,M-1
                    X=X+Z
                    IF(B(I,Y).NE.ZERO)TEMPA=TEMPA+A(X)*B(I,Y)
                    Y=Y+1
                ENDDO
                C(I,J)=C(I,J)+TEMPA
            ENDDO
        ENDDO
        RETURN
    ENDIF

    IF(PTYPE.EQ.0001)THEN
        DO J=1,DIMB
            DO I=1,DIMA
                TEMPA=ZERO
                TEMPB=(J-1)*J/2
                Y=1
                DO X=TEMPB+1,TEMPB+J
                    IF(B(Y,I).NE.ZERO)TEMPA=TEMPA+A(X)*B(Y,I)
                    Y=Y+1
                ENDDO
                X=X-1
                DO Z=J,M-1
                    X=X+Z
                    IF(B(Y,I).NE.ZERO)TEMPA=TEMPA+A(X)*B(Y,I)
                    Y=Y+1
                ENDDO
                C(I,J)=C(I,J)+TEMPA
            ENDDO
        ENDDO
        RETURN
    ENDIF

    IF(PTYPE.EQ.0010)THEN
        DO J=1,DIMB
            DO I=1,DIMA
                TEMPA=ZERO
                TEMPB=(J-1)*J/2
                Y=1
                DO X=TEMPB+1,TEMPB+J
                    IF(B(I,Y).NE.ZERO)TEMPA=TEMPA+A(X)*B(I,Y)
                    Y=Y+1
                ENDDO
                X=X-1
                DO Z=J,M-1
                    X=X+Z
                    IF(B(I,Y).NE.ZERO)TEMPA=TEMPA+A(X)*B(I,Y)
                    Y=Y+1
                ENDDO
                C(I,J)=C(I,J)+TEMPA*ALPHA
            ENDDO
        ENDDO
        RETURN
    ENDIF

    IF(PTYPE.EQ.0000)THEN
        DO J=1,DIMB
            DO I=1,DIMA
                TEMPA=ZERO
                TEMPB=(J-1)*J/2
                Y=1
                DO X=TEMPB+1,TEMPB+J
                    IF(B(Y,I).NE.ZERO)TEMPA=TEMPA+A(X)*B(Y,I)
                    Y=Y+1
                ENDDO
                X=X-1
                DO Z=J,M-1
                    X=X+Z
                    IF(B(Y,I).NE.ZERO)TEMPA=TEMPA+A(X)*B(Y,I)
                    Y=Y+1
                ENDDO
                C(I,J)=C(I,J)+TEMPA*ALPHA
            ENDDO
        ENDDO
        RETURN
    ENDIF

    END