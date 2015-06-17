
#include <algorithm>
#include <iostream>
#include <cstring>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

class Wordlist {
private:
    const char *filename
    const char *addr;
    size_t sz;
    int fd;



protected:

    void error(const char* msg) {
        perror(msg); 
        exit(255);
    }

    void unmap() {
        munmap(addr);
    }

    void map(const char* fname) {
        fd = open(fname, O_RDONLY);
        if (fd == -1)
            error("open");

        // obtain file size
        struct stat sb;
        if (fstat(fd, &sb) == -1)
            error("fstat");

        sz = sb.st_size;

        addr = static_cast<const char*>(mmap(NULL, sz, PROT_READ, MAP_PRIVATE, fd, 0u));
        if (addr == MAP_FAILED)
            error("mmap");

        // TODO close fd at some point in time, call munmap(...)
        return addr;
    }

    static uintmax_t fastLoad(char const *fname) {
        static const auto BUFFER_SIZE = 16*1024;
        int fd = open(fname, O_RDONLY);
        if(fd == -1)
            handle_error("open");

        /* Advise the kernel of our access pattern.  */
        posix_fadvise(fd, 0, 0, 1);  // FDADVICE_SEQUENTIAL

        char buf[BUFFER_SIZE + 1];
        uintmax_t lines = 0;

        while(size_t bytes_read = read(fd, buf, BUFFER_SIZE))
        {
            if(bytes_read == (size_t)-1)
                handle_error("read failed");
            if (!bytes_read)
                break;

            for(char *p = buf; (p = (char*) memchr(p, '\n', (buf + bytes_read) - p)); ++p)
                ++lines;
        }

        return lines;
    }

public:

    Wordlist() {
    }

    ~Wordlist() {
        this->unmap();
    }

    void load(const char *filename) {
        this->filename = filename;
        this->map(filename)
    }

}