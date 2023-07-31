#pragma once
#include <memory>
#include <string>
#include <stdexcept>
#include <type_traits>

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
    void failOutOfRange(std::size_t n, std::size_t size) const {
        throw std::out_of_range(
            (name + "[" + std::to_string(n) +
             "] is out of range (size = " + std::to_string(size) + ")")
                .c_str());
    }
    virtual void updateName(){}
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
        return *this;
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
template <typename T, std::size_t N>
class array : public NpChkBase, public std::array<T, N> {
    using Base = std::array<T, N>;
    Base &base() { return *this; }
    const Base &base() const { return *this; }
    void check(std::size_t n) const {
        if (n >= N) {
            failOutOfRange(n, N);
        }
    }

  public:
    array() = default;
    explicit array(const char *names) : NpChkBase(names), Base() {
        updateName();
    }
    void updateName() override{
        if constexpr(std::is_base_of_v<NpChkBase, T>){
            for (std::size_t n = 0; n < N; ++n) {
                base()[n].name = this->name + "[" + std::to_string(n) + "]";
                base()[n].updateName();
            }
        }
    }
    template <typename U>
    auto operator=(const U &rhs) {
        base() = rhs;
        return *this;
    }
    auto &at(std::size_t n) {
        check(n);
        return base()[n];
    }
    const auto &at(std::size_t n) const {
        check(n);
        return base()[n];
    }
    auto &operator[](std::size_t n) { return at(n); }
    const auto &operator[](std::size_t n) const { return at(n); }
    auto &front() { return at(0); }
    const auto &front() const { return at(0); }
    auto &back() { return at(N ? N - 1 : 0); }
    const auto &back() const { return at(N ? N - 1 : 0); }
    // 書きかけ
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
#define NPCHK_GETNAME8(_1, _2, _3, _4, _5, _6, _7, _8, NAME, ...) NAME
// https://stackoverflow.com/questions/24481810/how-to-remove-the-enclosing-parentheses-with-macro
#define NPCHK_DEPAREN(X) NPCHK_ESC(NPCHK_V2 X)
#define NPCHK_V2(...) NPCHK_V2 __VA_ARGS__
#define NPCHK_ESC(...) NPCHK_ESC2(__VA_ARGS__)
#define NPCHK_ESC2(...) NPCHK_V_##__VA_ARGS__
#define NPCHK_V_NPCHK_V2
#define NPCHK(type, ...)                                                       \
    NPCHK_DEPAREN(type)                                                        \
    NPCHK_GETNAME8(__VA_ARGS__, NPCHK8, NPCHK7, NPCHK6, NPCHK5, NPCHK4,        \
                   NPCHK3, NPCHK2, NPCHK1, )                                   \
    (#__VA_ARGS__, __VA_ARGS__)
