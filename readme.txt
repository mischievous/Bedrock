    DATE
(2022/11/11)            with ...
(2022/11/11)                mutex
(2022/11/11)                semaphore
(2022/11/11)                file (fopen)
(2022/11/11)                open


(____/__/__)            bedrock_make -- PHASE 1 (GET IT WORKING)
(2022/11/11)                ranlib    (libtool)
(2022/11/11)                libtool   (libtool)
(2022/11/11)                compile   (cc, cxx)
(2022/11/11)                link
(2022/11/12)                archiver  (ar)
(2022/11/12)                assembler (as)
(2022/11/12)                fortran   (fc)

(2022/11/12)            bedrock_make -- PHASE 2 (ARGPARSE)
(2022/11/12)                ranlib    (libtool)
(2022/11/12)                libtool   (libtool)
(2022/11/12)                compile   (cc, cxx)
(2022/11/12)                link
(2022/11/12)                archiver  (ar)
(2022/11/12)                assembler (as)
(2022/11/12)                fortran   (fc)

(____/__/__)            bedrock_make -- PHASE 2 ( compiler only)
(____/__/__)                compile   (cc, cxx)
(____/__/__)                link 
(____/__/__)                assembler (as)
(____/__/__)                fortran   (fc)


(____/__/__)            All path formats (actual:host)
(____/__/__)                actual (imported from xcode, make)
(____/__/__)                host   (optional)

(____/__/__)            json file format
(____/__/__)                targetType (TARGET/SOURCE)
(____/__/__)                path (relative to project path)
(2022/11/11)                defines
(2022/11/11)                include paths -- includePath
(2022/11/11)                library paths
(2022/11/11)                libaries
(2022/11/11)                dependancies
(____/__/__)                pch files

(____/__/__)            integrated remote file system
(____/__/__)                sshfs
(____/__/__)                bundle -- (cross compile -- include/libs/...)
(____/__/__)                build  -- destination?


(____/__/__)            clang pch
(2022/11/11)                https://clang.llvm.org/docs/PCHInternals.html
(____/__/__)                only system includes
(____/__/__)                add to json pch files

(____/__/__)            source dependancy scanner


(____/__/__)            import makefile
(____/__/__)                gui
(____/__/__)                    local/remote/cross
(____/__/__)                        remote -- ssh
(____/__/__)                        remote -- target type
(____/__/__)                        cross  -- bundle for cross compile builder
(____/__/__)                    make "clean" target --> "clean"
(____/__/__)                    make "build" target --> "all" or ""
(____/__/__)                    additional tools to capture and path to captured tool


(____/__/__)            import xcode
(____/__/__)                gui

