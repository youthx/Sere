# Use a base image with LLVM, Clang, and CMake
FROM mcr.microsoft.com/devcontainers/cpp:0-ubuntu-22.04

# Install additional LLVM tools and dependencies
RUN apt-get update && apt-get install -y \
    clang \
    clang-tidy \
    lldb \
    llvm \
    llvm-dev \
    lld \
    ninja-build \
    cmake \
    git \
    unzip \
    zip \
    curl \
    && apt-get clean

# Optional: Set default compiler to Clang
ENV CC=clang
ENV CXX=clang++

# Pre-create build directory for convenience
WORKDIR /workspace
RUN mkdir -p /workspace/build