#pragma once

#include <unistd.h>
#include <fcntl.h>

#include <stdexcept>
#include <sstream>
#include <cerrno>

class PipeOpenError : public std::runtime_error
{
public:
    int errorCode() const { return err_; }
private:
    PipeOpenError(int err)
        : std::runtime_error(err_to_string(err)),
          err_(err)
    {}

    static std::string err_to_string(int err)
    {
        std::ostringstream o;
        bool detected = false;

        switch(err) {
            case EFAULT:
                o << "EFAULT ( " << err << "): pipefd is not valid.";
                detected = true;
                break;
            case EINVAL:
                o << "EFAULT ( " << err << "): Invalid value in flags.";
                detected = true;
                break;
            case EMFILE:
                o << "EMFILE ( " << err << "): Too many file descriptors "
                  << "are in use by process.";
                detected = true;
                break;
            case ENFILE:
                o << "ENFILE ( " << err << "): The system limits on the "
                  << "total number of open files has been reached.";
                detected = true;
                break;
        }

        if(!detected) {
            o << "Unrecognized error! (" << err << ")";
        }

        return o.str();
    }

    int err_;
    friend class NonblockingPipe;
};

class ReadPipeError : public std::runtime_error
{
public:
    int errorCode() const { return err_; }
private:
    ReadPipeError(int err)
        : std::runtime_error(err_to_string(err)),
          err_(err)
    {}

    static std::string err_to_string(int err)
    {
        std::ostringstream o;
        bool detected = false;

        switch(err) {
            case EBADF:
                o << "EBADF ( " << err << "): Invalid file descriptor or "
                  << "not open for reading.";
                detected = true;
                break;
            case EFAULT:
                o << "EFAULT ( " << err << "): buffer is outside your "
                  << "accessible address space.";
                detected = true;
                break;
            case EINTR:
                // We handle this by just returning 0 bytes read.
                assert(false);
                break;
            case EINVAL:
                o << "EINVAL ( " << err << "): Descriptor is attached to "
                  << "an object insuitable for reading, or the file was "
                  << "opened with the O_DIRECT flag and either the address "
                  << "specified for the buffer, the value specified in "
                  << "count, or the file offset are not suitably aligned.";
                detected = true;
                break;
            case EIO:
                o << "EIO ( " << err << "): I/O error. There may be a low "
                  << "level I/O problem.";
                detected = true;
                break;
        }

        if(!detected) {
            o << "Unrecognized error! (" << err << ")";
        }

        return o.str();
    }

    int err_;
    friend class ReadPipeHandle;
};

class WritePipeError : public std::runtime_error
{
public:
    int errorCode() const { return err_; }
private:
    WritePipeError(int err)
        : std::runtime_error(err_to_string(err)),
          err_(err)
    {}

    static std::string err_to_string(int err)
    {
        std::ostringstream o;
        bool detected = false;

        switch(err) {
            case EBADF:
                o << "EBADF ( " << err << "): Invalid file descriptor or "
                  << "not open for reading.";
                detected = true;
                break;
            case EFAULT:
                o << "EFAULT ( " << err << "): buffer is outside your "
                  << "accessible address space.";
                detected = true;
                break;
            case EINTR:
                // We handle this by just returning 0 bytes read.
                assert(false);
                break;
            case EINVAL:
                o << "EINVAL ( " << err << "): Descriptor is attached to "
                  << "an object insuitable for reading, or the file was "
                  << "opened with the O_DIRECT flag and either the address "
                  << "specified for the buffer, the value specified in "
                  << "count, or the file offset are not suitably aligned.";
                detected = true;
                break;
            case EIO:
                o << "EIO ( " << err << "): I/O error. There may be a low "
                  << "level I/O problem.";
                detected = true;
                break;
            case ENOSPC:
                o << "ENOSPC ( " << err << "): The device containing the "
                  << "pipe has no room for the data.";
                detected = true;
                break;
            case EPIPE:
                o << "EPIPE ( " << err << "): The reading end of the pipe "
                  << "has closed.";
                detected = true;
                break;
        }

        if(!detected) {
            o << "Unrecognized error! (" << err << ")";
        }

        return o.str();
    }

    int err_;
    friend class WritePipeHandle;
};

class ReadPipeHandle
{
public:
    std::size_t read(void* buf, std::size_t count)
    {
        std::size_t s = ::read(read_fd_, buf, count);

        if(s == -1) {
            if(errno == EINTR)
                return 0;
            throw ReadPipeError(errno);
        }

        return s;
    }

private:
    ReadPipeHandle(int read_fd)
        : read_fd_(read_fd)
    {}

    int read_fd_;

    friend class NonblockingPipe;
};

class WritePipeHandle
{
public:
    std::size_t write(void const* buf, std::size_t count)
    {
        std::size_t s = ::write(write_fd_, buf, count);

        if(s == -1) {
            if(errno == EINTR)
                return 0;
            throw WritePipeError(errno);
        }

        return s;
    }

private:
    WritePipeHandle(int write_fd)
        : write_fd_(write_fd)
    {}

    int write_fd_;

    friend class NonblockingPipe;
};

class NonblockingPipe
{
public:
    NonblockingPipe(bool close_on_exec)
    {
        int err = pipe2(pipe_fds_, O_NONBLOCK);
        if(err)
            throw PipeOpenError(err);
    }

private:
    int pipe_fds_[2];
};

