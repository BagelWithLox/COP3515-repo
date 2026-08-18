###############################################################################
# COP3515 Programming Environment
#
# University Programming Environment
#
# Languages
#   • C17
#   • Rust (stable)
#
# IDE
#   • Visual Studio Code Dev Containers
#
# Supported Tools
#   • GCC
#   • Clang
#   • GDB
#   • LLDB
#   • Make
#   • CMake
#   • Cargo
#   • Rust Analyzer
#   • Clippy
#   • Rustfmt
#   • Valgrind
#   • Cppcheck
#   • Doxygen
###############################################################################

FROM ubuntu:24.04

LABEL maintainer="Your University"
LABEL description="COP3515 Programming Environment"

###############################################################################
# Prevent interactive package installation
###############################################################################

ENV DEBIAN_FRONTEND=noninteractive

###############################################################################
# Update Ubuntu
###############################################################################

RUN apt-get update && \
    apt-get upgrade -y

###############################################################################
# Install Linux packages
###############################################################################

RUN apt-get install -y \
    build-essential \
    gcc \
    g++ \
    gdb \
    clang \
    clangd \
    clang-format \
    clang-tidy \
    lldb \
    lld \
    make \
    cmake \
    ninja-build \
    valgrind \
    cppcheck \
    git \
    curl \
    wget \
    unzip \
    zip \
    tree \
    nano \
    vim \
    less \
    file \
    python3 \
    python3-pip \
    python3-venv \
    graphviz \
    doxygen \
    pandoc \
    sudo \
    locales \
    pkg-config \
    openssl \
    libssl-dev

###############################################################################
# Configure locale
###############################################################################

RUN locale-gen en_US.UTF-8

ENV LANG=en_US.UTF-8
ENV LC_ALL=en_US.UTF-8

###############################################################################
# Create student account
###############################################################################

RUN useradd -m -s /bin/bash student

RUN echo "student ALL=(ALL) NOPASSWD:ALL" >/etc/sudoers.d/student

###############################################################################
# Workspace
###############################################################################

RUN mkdir -p /workspace

RUN chown -R student:student /workspace

###############################################################################
# Install Rust (as student)
###############################################################################

USER student

WORKDIR /home/student

RUN curl https://sh.rustup.rs -sSf | \
    sh -s -- -y --profile default

ENV PATH="/home/student/.cargo/bin:${PATH}"

###############################################################################
# Install Rust components
###############################################################################

RUN rustup component add \
    rustfmt \
    clippy \
    rust-src

###############################################################################
# Useful Cargo utilities
###############################################################################

RUN cargo install \
    cargo-edit \
    cargo-watch \
    cargo-expand \
    cargo-audit \
    cargo-outdated

###############################################################################
# Verify installations
###############################################################################

RUN gcc --version

RUN clang --version

RUN gdb --version

RUN lldb --version

RUN rustc --version

RUN cargo --version

RUN cargo clippy --version

RUN cargo fmt --version

RUN cmake --version

RUN make --version

RUN python3 --version

###############################################################################
# Workspace
###############################################################################

WORKDIR /workspace

###############################################################################
# Keep container alive
###############################################################################

CMD ["sleep", "infinity"]