#!/bin/bash

GS=$(ls -d *game 2>/dev/null | head -n 1)
NET=$(ls -d *net 2>/dev/null | head -n 1)
SKILL=$(ls -d *skill 2>/dev/null | head -n 1)

GS=${GS:-cgame}
NET=${NET:-cnet}
SKILL=${SKILL:-cskill}

CORES=$(nproc 2>/dev/null || echo 4)

print_msg() {
    echo -e "\n=========================== $1 ===========================\n"
}

install_deps() {
    print_msg "Installing required dependencies"

    export DEBIAN_FRONTEND=noninteractive

    dpkg --add-architecture i386
    apt-get update -y && apt-get upgrade -y

    apt-get install -y mc screen htop openjdk-11-jre mono-complete exim4 p7zip-full libpcap-dev curl wget ipset net-tools tzdata ntpdate mariadb-server mariadb-client
    apt-get install -y make gcc g++ libssl-dev:i386 libssl-dev libcrypto++-dev libpcre3 libpcre3-dev libpcre3:i386 libpcre3-dev:i386 libtesseract-dev libx11-dev:i386 libx11-dev gcc-multilib libc6-dev:i386 build-essential g++-multilib libtemplate-plugin-xml-perl libxml2-dev libxml2-dev:i386 libxml2:i386 libstdc++6:i386 libmariadb-dev-compat:i386 libmariadb-dev:i386
    apt-get install -y libdb++-dev:i386 libdb-dev:i386 libdb5.3:i386 libdb5.3++:i386 libdb5.3++-dev:i386 libdb5.3-dbg:i386 libdb5.3-dev:i386
    apt-get install -y libdb++-dev libdb-dev libdb5.3 libdb5.3++ libdb5.3++-dev libdb5.3-dbg libdb5.3-dev
    apt-get install -y libmysqlcppconn-dev libjsoncpp-dev libmariadb-dev-compat curl libcurl4:i386 pkg-config libcurl4-openssl-dev
}

# Generic: clean + parallel build of a directory.
# Optional second arg = make target (e.g. "lib").
build_dir() {
    local dir=$1
    local target=${2:-}
    if [ -d "$dir" ]; then
        print_msg "Building $dir"
        pushd "$dir" > /dev/null
        make clean > /dev/null 2>&1 || true
        make -j"$CORES" $target
        popd > /dev/null
    else
        echo "WARNING: Directory $dir not found, skipping."
    fi
}

setup_env() {
    print_msg "Setting up $NET"
    pushd "$NET" > /dev/null
    rm -f common io mk storage rpc lua rpcgen
    ln -sf ~/share/common/ .
    ln -sf ~/share/io/ .
    ln -sf ~/share/mk/ .
    ln -sf ~/share/storage/ .
    ln -sf ~/share/rpc/ .
    ln -sf ~/share/lua/ .
    ln -sf ~/share/rpcgen .
    popd > /dev/null

    print_msg "Setting up $GS"
    pushd "$GS" > /dev/null
    rm -f lua liblua.a
    ln -sf ~/share/lua/ .
    # Rules.make references $(BASEPATH)/liblua.a — create a direct symlink
    # so cgame/gs can find liblua.a without knowing the lua/src/ sub-path.
    ln -sf lua/src/liblua.a liblua.a
    popd > /dev/null

    print_msg "Setting up $SKILL"
    pushd "$SKILL" > /dev/null
    rm -f mk
    ln -sf ~/share/mk/ .
    ln -sf ~/share/io/ .
    ln -sf ~/share/common/ .
	ln -sf ../../"$GS"/gs/attack.h header/attack.h
	ln -sf ../../"$GS"/gs/obj_interface.h header/obj_interface.h
    popd > /dev/null

    print_msg "Setting up iolib"
    mkdir -p iolib/inc

    pushd iolib/inc > /dev/null
    rm -f *
    ln -sf ../../$NET/gamed/auctionsyslib.h .
    ln -sf ../../$NET/gamed/sysauctionlib.h .
    ln -sf ../../$NET/gdbclient/db_if.h .
    ln -sf ../../$NET/gamed/factionlib.h .
    ln -sf ../../$NET/common/glog.h .
    ln -sf ../../$NET/gamed/gsp_if.h .
    ln -sf ../../$NET/gamed/mailsyslib.h .
    ln -sf ../../$NET/gamed/privilege.hxx .
    ln -sf ../../$NET/gamed/sellpointlib.h .
    ln -sf ../../$NET/gamed/stocklib.h .
    ln -sf ../../$NET/gamed/webtradesyslib.h .
    ln -sf ../../$NET/gamed/kingelectionsyslib.h .
    ln -sf ../../$NET/gamed/pshopsyslib.h .
    ln -sf ../../$NET/gdbclient/db_os.h .
    ln -sf ~/share/io/luabase.h .
    popd > /dev/null

    pushd iolib > /dev/null
    rm -f lib*
    ln -sf ../$NET/io/libgsio.a .
    ln -sf ../$NET/gdbclient/libdbCli.a .
    ln -sf ../$SKILL/skill/libskill.a .
    ln -sf ../$NET/gamed/libgsPro2.a .
    ln -sf ../$NET/logclient/liblogCli.a .
    popd > /dev/null

    print_msg "Modifying Rules.make"
    local EPWD=$(pwd | sed -e 's/\//\\\//g')
    sed -i -e "s/IOPATH=.*$/IOPATH=$EPWD\/iolib/g" -e "s/BASEPATH=.*$/BASEPATH=$EPWD\/$GS/g" "$GS/Rules.make"

    print_msg "Softlinking libskill.so"
    rm -f "$GS/gs/libskill.so"
    ln -sf ../../$SKILL/libskill.so "$GS/gs/libskill.so"
}

# Build liblua.a for a given base directory (cnet or cskill).
# Must be called before any module that links against SHARE_SOBJ.
build_lua() {
    local base_dir=$1
    local lua_src="$base_dir/lua/src"

    if [ ! -d "$lua_src" ]; then
        echo "WARNING: lua source not found at $lua_src, skipping."
        return 0
    fi

    print_msg "Building lua ($lua_src)"
    pushd "$lua_src" > /dev/null
    make clean > /dev/null 2>&1 || true
    make linux
    popd > /dev/null
}

build_rpcgen() {
    print_msg "$NET rpcgen"
    pushd "$NET" > /dev/null
    chmod +x rpcgen
    chmod -R +x rpc/
    ./rpcgen rpcalls.xml
    popd > /dev/null
}

# Build all cnet/cskill libraries that cgame needs via iolib, then
# extract them into cgame/libgs/ and build cgame/libcm.
#
# Dependency chain for libgsio.a (the tricky one):
#   cnet/io "lib" target just does:  ar crs libgsio.a $(OUTEROBJS_M) $(OBJS_M)
#   It does NOT compile the _m.o files — it only archives pre-existing ones.
#   cnet/gamed "lib" depends on $(SHAREOBJ) = *_m.o  →  it DOES compile them.
#   Therefore the correct order is:
#     1. gdbclient lib  (cleans _m.o, builds its own objects — no SHAREOBJ dep)
#     2. gamed    lib  (cleans then COMPILES _m.o as prerequisites → they exist)
#     3. cnet/io  "make lib" WITHOUT clean → ar picks up the freshly compiled _m.o
build_gslib() {
    # ── logclient (SINGLE_THREAD=true → uses Makefile.gs, does not touch _m.o) ──
    print_msg "Building $NET/logclient lib → liblogCli.a"
    pushd "$NET/logclient" > /dev/null
    make clean > /dev/null 2>&1 || true
    make -f Makefile.gs clean > /dev/null 2>&1 || true
    make -f Makefile.gs -j"$CORES"
    popd > /dev/null

    # ── gdbclient lib → libdbCli.a ──────────────────────────────────────────────
    # lib: $(LIBOBJS)  — no SHAREOBJ dep, so _m.o files get cleaned but not rebuilt
    build_dir "$NET/gdbclient" "lib"

    # ── gamed lib → libgsPro2.a  (also compiles *_m.o as prerequisite) ──────────
    # lib: $(LIBOBJS) $(SHAREOBJ) $(OUTEROBJS)
    # $(SHAREOBJ) = *_m.o  →  make compiles all _m.o into cnet/io/ and cnet/common/
    build_dir "$NET/gamed" "lib"

    # ── cnet/io lib → libgsio.a  (archive the _m.o files gamed just compiled) ───
    # Do NOT use build_dir here — that would clean _m.o before archiving.
    print_msg "Building $NET/io lib → libgsio.a"
    pushd "$NET/io" > /dev/null
    make lib
    popd > /dev/null

    # ── cgame/libcm → cgame/libcm.a ─────────────────────────────────────────────
    # Referenced in Rules.make CMLIB but NOT built by cgame/Makefile — must be
    # built explicitly. cgame/Makefile clean does not remove libcm.a (libcm is not
    # in CLEAN_SUBDIR), so libcm.a survives the make clean inside build_game.
    build_dir "$GS/libcm"

    # ── Extract all iolib archives into cgame/libgs/{io,gs,db,sk,log}/ ──────────
    # cgame/Makefile "all" also does this via the gslib target, but doing it here
    # lets us catch missing archives early (before the long gs compile).
    print_msg "Extracting iolib into $GS/libgs"
    mkdir -p "$GS/libgs/"{io,gs,db,sk,log}
    pushd "$GS/libgs" > /dev/null
    make -j"$CORES"
    popd > /dev/null
}

build_skill() {
    build_dir "$SKILL/skill"
    build_dir "$SKILL"
}

# Build the game server binary.
# cgame/Makefile "all":  lib → collision → solib → gs
#   lib:      builds cgame/io/pollio.o, re-extracts libgs/, creates libonline.a
#   collision: builds libTrace.a
#   solib:    builds libtask.so
#   gs:       links everything (needs libcm.a, libonline.a, libgs/*.o, liblua.a, libTrace.a)
build_game() {
    build_dir "$GS"
}

build_deliver() {
    build_dir "$NET/common"
    build_dir "$NET/io"

    local modules=("gauthd" "logservice" "gacd" "glinkd" "gdeliveryd" "gamedbd" "uniquenamed" "gfaction")
    for mod in "${modules[@]}"; do
        build_dir "$NET/$mod"
    done
}

install_func() {
    print_msg "Installing daemons"

    mkdir -p /home/{gamed,gfactiond,gauthd,uniquenamed,gamedbd,gdeliveryd,glinkd,gacd,logservice}

    cp -f "$GS/gs/gs" /home/gamed/gs
    cp -f "$GS/gs/libtask.so" /home/gamed/libtask.so
    cp -f "$SKILL/libskill.so" /home/gamed/libskill.so
    cp -f "$NET/gfaction/gfactiond" /home/gfactiond/gfactiond
    cp -f "$NET/gauthd/gauthd" /home/gauthd/gauthd
    cp -f "$NET/uniquenamed/uniquenamed" /home/uniquenamed/uniquenamed
    cp -f "$NET/gamedbd/gamedbd" /home/gamedbd/gamedbd
    cp -f "$NET/gdeliveryd/gdeliveryd" /home/gdeliveryd/gdeliveryd
    cp -f "$NET/glinkd/glinkd" /home/glinkd/glinkd
    cp -f "$NET/gacd/gacd" /home/gacd/gacd
    cp -f "$NET/logservice/logservice" /home/logservice/logservice

    print_msg "Success"
}

install_protect_func() {
    print_msg "Moving to get_protects"

    mkdir -p /root/get_protects

    cp -f "$GS/gs/gs" /root/get_protects/gs
    cp -f "$NET/gfaction/gfactiond" /root/get_protects/gfactiond
    cp -f "$NET/gauthd/gauthd" /root/get_protects/gauthd
    cp -f "$NET/uniquenamed/uniquenamed" /root/get_protects/uniquenamed
    cp -f "$NET/gamedbd/gamedbd" /root/get_protects/gamedbd
    cp -f "$NET/gdeliveryd/gdeliveryd" /root/get_protects/gdeliveryd
    cp -f "$NET/glinkd/glinkd" /root/get_protects/glinkd

    print_msg "Success"
}

clean_obj_files() {
	print_msg "Cleaning up object (.o) files"
	find "$GS" "$NET" "$SKILL" iolib -type f -name "*.o" -delete 2>/dev/null
	print_msg "Cleanup complete"
}

case "$1" in
    deliver|deliveryd)
        # Build cnet daemons (gauthd, logservice, glinkd, etc.)
        build_lua "$NET"
        build_rpcgen
        build_deliver
        ;;
    skill)
        # Build cskill shared library
        build_lua "$SKILL"
        build_skill
        ;;
    gs)
        # Build game server (cgame/gs).
        # Requires: lua, skill (libskill.a for iolib), and all cnet libs.
		setup_env
        build_lua "$NET"
        build_lua "$SKILL"
        build_skill          # libskill.a → iolib/libskill.a (symlink)
        build_gslib          # cnet libs + cgame/libcm + libgs extraction
        build_game           # cgame/Makefile all → gs binary
		clean_obj_files
        ;;
    all)
        print_msg "Starting Build All process"
        install_deps
        setup_env
        # 1. Build shared lua library (cnet/lua → ~/share/lua)
        build_lua "$NET"
        # 2. Build cnet delivery daemons
        build_rpcgen
        build_deliver
        # 3. Build cskill and its lua
        build_lua "$SKILL"
        build_skill          # libskill.a must exist before build_gslib
        # 4. Build cgame prerequisites and game server
        build_gslib
        build_game
        # 5. Deploy
        install_func
        install_protect_func
		clean_obj_files
        ;;
    install)
        install_func
        install_protect_func
        ;;
    deps)
        install_deps
        ;;
	clean)
		clean_obj_files
        ;;
    *)
        echo "Invalid command!"
        echo "Usage: $0 {deliver|skill|gs|all|install|deps}"
        exit 1
        ;;
esac