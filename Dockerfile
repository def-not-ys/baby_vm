FROM alpine
RUN apk add --no-cache build-base make
RUN apk add --no-cache clang clang-dev gcc lld musl-dev
WORKDIR /baby_vm
COPY . .
RUN make clean && make
CMD ["./baby_vm"]