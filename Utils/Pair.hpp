#ifndef SUPERHEROES_PAIR_HPP
#define SUPERHEROES_PAIR_HPP

#include <utility>

template<typename F, typename S>
class Pair
{
    F m_first;
    S m_second;
public:
    Pair() = default;
    Pair(const F& first, const S& second);
    Pair(F&& first, S&& second);

    const F &GetFirst() const;
    const S &GetSecond() const;

    void SetFirst(const F &f);
    void SetFirst(F &&f);
    void SetSecond(const S &s);
    void SetSecond(S &&s);
};

template<typename F, typename S>
Pair<F, S>::Pair(const F& first, const S& second)
{
    m_first = first;
    m_second = second;
}

template<typename F, typename S>
Pair<F, S>::Pair(F &&first, S &&second)
{
    m_first = std::move(first);
    m_second = std::move(second);
}

template<typename F, typename S>
const F &Pair<F, S>::GetFirst() const
{
    return m_first;
}

template<typename F, typename S>
const S &Pair<F, S>::GetSecond() const
{
    return m_second;
}

template<typename F, typename S>
void Pair<F, S>::SetFirst(const F &f)
{
    m_first = f;
}

template<typename F, typename S>
void Pair<F, S>::SetFirst(F &&f)
{
    m_first = std::move(f);
}

template<typename F, typename S>
void Pair<F, S>::SetSecond(const S &s)
{
    m_second = s;
}

template<typename F, typename S>
void Pair<F, S>::SetSecond(S &&s)
{
    m_second = std::move(s);
}

#endif
