#include <memory>
#include <string>
#include <stdexcept>

namespace npchk {
std::string nameParse(const char *names) {
    static const char *current_names, *current_names_pos;
    if (names != current_names) {
        current_names = names;
        current_names_pos = names;
    }
    const char *start = current_names_pos;

    while (*current_names_pos != ',' && *current_names_pos != 0) {
        ++current_names_pos;
    }
    const char *end = current_names_pos;
    ++current_names_pos; // ','
    while (*current_names_pos == ' ' || *current_names_pos == 0) {
        ++current_names_pos;
    }
    return std::string(start, static_cast<std::string::size_type>(end - start));
}

struct NpChkBase {
    std::string name;
    NpChkBase(const char *names) : name(nameParse(names)) {}
    void failNullPtr() {
        throw std::runtime_error((name + " is nullptr").c_str());
    }
};
template <typename T>
class shared_ptr : public std::shared_ptr<T>, NpChkBase {
    void check() {
        if (static_cast<std::shared_ptr<T> &>(*this) == nullptr) {
            failNullPtr();
        }
    }

  public:
    shared_ptr(const char *names) : std::shared_ptr<T>(), NpChkBase(names) {}
    template <typename U>
    auto operator=(const U &rhs) {
        static_cast<std::shared_ptr<T> &>(*this) = rhs;
    }
    operator std::shared_ptr<T>() {
        check();
        std::cout << "cast" << std::endl;
        return static_cast<std::shared_ptr<T>>(
            static_cast<std::shared_ptr<T> &>(*this));
    }
    operator const std::shared_ptr<T> &() const {
        check();
        std::cout << "cast" << std::endl;
        return static_cast<std::shared_ptr<T>>(
            static_cast<std::shared_ptr<T> &>(*this));
    }
    auto get() {
        check();
        return this->std::shared_ptr<T>::get();
    }
    auto operator*() { return *get(); }
    auto operator->() { return get(); }
    auto operator[](std::ptrdiff_t i) { return get()[i]; }
};
} // namespace npchk

#define NPCHK1(A, v1) v1(A)
#define NPCHK2(A, v1, v2) v1(A), v2(A)
#define NPCHK3(A, v1, v2, v3) v1(A), v2(A), v3(A)
#define NPCHK4(A, v1, v2, v3, v4) v1(A), v2(A), v3(A), v4(A)
#define NPCHK5(A, v1, v2, v3, v4, v5) v1(A), v2(A), v3(A), v4(A), v5(A)
#define NPCHK6(A, v1, v2, v3, v4, v5, v6)                                      \
    v1(A), v2(A), v3(A), v4(A), v5(A), v6(A)
#define NPCHK7(A, v1, v2, v3, v4, v5, v6, v7)                                  \
    v1(A), v2(A), v3(A), v4(A), v5(A), v6(A), v7(A)
#define NPCHK8(A, v1, v2, v3, v4, v5, v6, v7, v8)                              \
    v1(A), v2(A), v3(A), v4(A), v5(A), v6(A), v7(A), v8(A)
#define GETNAME8(_1, _2, _3, _4, _5, _6, _7, _8, NAME, ...) NAME
#define NPCHK(type, ...)                                                       \
    type GETNAME8(__VA_ARGS__, NPCHK8, NPCHK7, NPCHK6, NPCHK5, NPCHK4, NPCHK3, \
                  NPCHK2, NPCHK1)(#__VA_ARGS__, __VA_ARGS__)
