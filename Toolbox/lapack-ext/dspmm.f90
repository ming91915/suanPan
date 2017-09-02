    !
    ! \param[in] UPLO
    ! \param[in] M
    ! \param[in] N
    ! \param[in] ALPHA
    ! \param[in] A
    ! \param[in] B
    ! \param[in] LDB
    ! \param[in] BETA
    ! \param[in] C
    ! \param[in] LDC
    !
    SUBROUTINE DSPMM(UPLO,M,N,ALPHA,A,B,LDB,BETA,C,LDC)

    CHARACTER UPLO
    INTEGER M,N,LDB,LDC
    DOUBLEPRECISION ALPHA,BETA,A(*),B(LDB,*),C(LDC,*)

    INTEGER I,J,K,X,Y,Z
    DOUBLEPRECISION ZERO,ONE,TEMPA,TEMPB
    PARAMETER (ZERO=0.0D+0,ONE=1.0D+0)

    IF(ALPHA.EQ.ZERO)THEN
        IF(BETA.EQ.ZERO)THEN
            DO J=1,N
                DO I=1,M
                    C(I,J)=ZERO
                ENDDO
            ENDDO
        ELSEIF(BETA.NE.ONE)THEN
            DO J=1,N
                DO I=1,M
                    C(I,J)=BETA*C(I,J)
                ENDDO
            ENDDO
        ENDIF
        RETURN
    ENDIF

    IF(BETA.EQ.ZERO)THEN
        DO J=1,N
            DO I=1,M
                C(I,J)=ZERO
            ENDDO
        ENDDO
    ELSEIF(BETA.NE.ONE)THEN
        DO J=1,N
            DO I=1,M
                C(I,J)=BETA*C(I,J)
            ENDDO
        ENDDO
    ENDIF

    IF(LSAME(UPLO,'L'))THEN
        IF(ALPHA.EQ.ONE)THEN
            DO J=1,N
                DO I=1,M
                    TEMPA=ZERO
                    X=I
                    Y=1
                    DO K=M-1,M-I+1,-1
                        IF(B(Y,J).NE.ZERO)THEN
                            TEMPA=TEMPA+A(X)*B(Y,J)
                        ENDIF
                        X=X+K
                        Y=Y+1
                    ENDDO
                    DO Z=X,M-I+X
                        IF(B(Y,J).NE.ZERO)THEN
                            TEMPA=TEMPA+A(Z)*B(Y,J)
                        ENDIF
                        Y=Y+1
                    ENDDO
                    C(I,J)=C(I,J)+TEMPA
                ENDDO
            ENDDO
        ELSE
            DO J=1,N
                DO I=1,M
                    TEMPA=ZERO
                    X=I
                    Y=1
                    DO K=M-1,M-I+1,-1
                        IF(B(Y,J).NE.ZERO)THEN
                            TEMPA=TEMPA+A(X)*B(Y,J)
                        ENDIF
                        X=X+K
                        Y=Y+1
                    ENDDO
                    DO Z=X,M-I+X
                        IF(B(Y,J).NE.ZERO)THEN
                            TEMPA=TEMPA+A(Z)*B(Y,J)
                        ENDIF
                        Y=Y+1
                    ENDDO
                    C(I,J)=C(I,J)+TEMPA*ALPHA
                ENDDO
            ENDDO
        ENDIF
    ELSE
        IF(ALPHA.EQ.ONE)THEN
            DO J=1,N
                DO I=1,M
                    TEMPA=ZERO
                    TEMPB=(I-1)*I/2
                    Y=1
                    DO X=TEMPB+1,TEMPB+I
                        IF(B(Y,J).NE.ZERO)THEN
                            TEMPA=TEMPA+A(X)*B(Y,J)
                        ENDIF
                        Y=Y+1
                    ENDDO
                    X=X-1
                    DO Z=I,M-1
                        X=X+Z
                        IF(B(Y,J).NE.ZERO)THEN
                            TEMPA=TEMPA+A(X)*B(Y,J)
                        ENDIF
                        Y=Y+1
                    ENDDO
                    C(I,J)=C(I,J)+TEMPA
                ENDDO
            ENDDO
        ELSE
            DO J=1,N
                DO I=1,M
                    TEMPA=ZERO
                    TEMPB=(I-1)*I/2
                    Y=1
                    DO X=TEMPB+1,TEMPB+I
                        IF(B(Y,J).NE.ZERO)THEN
                            TEMPA=TEMPA+A(X)*B(Y,J)
                        ENDIF
                        Y=Y+1
                    ENDDO
                    X=X-1
                    DO Z=I,M-1
                        X=X+Z
                        IF(B(Y,J).NE.ZERO)THEN
                            TEMPA=TEMPA+A(X)*B(Y,J)
                        ENDIF
                        Y=Y+1
                    ENDDO
                    C(I,J)=C(I,J)+TEMPA*ALPHA
                ENDDO
            ENDDO
        ENDIF
    ENDIF

    END
