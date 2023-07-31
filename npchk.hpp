#include <memory>
#include <string>
#include <stdexcept>

namespace npchk {
struct NpChkBase {
    std::string name;
    NpChkBase() : name("(unknown variable)") {}
    explicit NpChkBase(const char *names) {
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
        if (*current_names_pos == ',') {
            ++current_names_pos;
            while (*current_names_pos == ' ' || *current_names_pos == 0) {
                ++current_names_pos;
            }
        }
        name = std::string(start,
                           static_cast<std::string::size_type>(end - start));
    }
    void failNullPtr() const {
        throw std::runtime_error((name + " is nullptr").c_str());
    }
};
template <typename T>
class shared_ptr : public NpChkBase {
    using Base = std::shared_ptr<T>;
    Base base;
    void check() const {
        if (base == nullptr) {
            failNullPtr();
        }
    }

  public:
    shared_ptr() = default;
    explicit shared_ptr(const char *names) : NpChkBase(names), base() {}
    template <typename U>
    auto operator=(const U &rhs) {
        base = rhs;
    }
    operator Base() const {
        check();
        return base;
    }
    auto get() const {
        check();
        return base.get();
    }
    auto operator*() const { return *get(); }
    auto operator->() const { return get(); }
    // auto operator[](std::ptrdiff_t i) const { return get()[i]; }
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
