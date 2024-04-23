#pragma once
#include <cmath>
// イージング関数
class Easing
{
private:
    template <class Ty = float>
    static constexpr Ty PaiF{ static_cast<Ty>(3.141592653589793f) }; // 円周率

    template <class Ty = double>
    static constexpr Ty Pai{ static_cast<Ty>(3.141592653589793) }; // 円周率

    template<typename Ty>
    static constexpr Ty _0 = static_cast<Ty>(0);
    template<typename Ty>
    static constexpr Ty _0_5 = static_cast<Ty>(0.5);
    template<typename Ty>
    static constexpr Ty _0_75 = static_cast<Ty>(0.75);
    template<typename Ty>
    static constexpr Ty _0_9375 = static_cast<Ty>(0.9375);
    template<typename Ty>
    static constexpr Ty _0_984375 = static_cast<Ty>(0.984375);

    template<typename Ty>
    static constexpr Ty _1 = static_cast<Ty>(1);
    template<typename Ty>
    static constexpr Ty _1_5 = static_cast<Ty>(1.5);
    template<typename Ty>
    static constexpr Ty _1_525 = static_cast<Ty>(1.525);

    template<typename Ty>
    static constexpr Ty _2 = static_cast<Ty>(2);
    template<typename Ty>
    static constexpr Ty _2_25 = static_cast<Ty>(2.25);
    template<typename Ty>
    static constexpr Ty _2_5 = static_cast<Ty>(2.5);
    template<typename Ty>
    static constexpr Ty _2_625 = static_cast<Ty>(2.625);
    template<typename Ty>
    static constexpr Ty _2_75 = static_cast<Ty>(2.75);

    template<typename Ty>
    static constexpr Ty _7_5625 = static_cast<Ty>(7.5625);

    template<typename Ty>
    static constexpr Ty _10 = static_cast<Ty>(10);

    template<typename Ty>
    static constexpr Ty _180 = static_cast<Ty>(180);

    template<typename Ty>
    static constexpr Ty _90 = static_cast<Ty>(90);

public:
    template<class Ty>
    [[nodiscard]] static inline Ty ToRadianF(const Ty angle)
    {
        return static_cast<Ty>(angle * PaiF<Ty> / _180<Ty>);
    }

private:
    template<class Ty>
    [[nodiscard]] static inline Ty ToRadian(const Ty angle)
    {
        return static_cast<Ty>(angle * Pai<Ty> / _180<Ty>);
    }

    template<typename Ty>
    [[nodiscard]] static inline bool AdjEqual(const Ty epsilon_num, const Ty num)
    {
        constexpr auto Fabs{ [](const Ty num) constexpr {
            if (num > _0<Ty>) return num; else return -num;
        } };

        static constexpr auto Epsilon{ std::numeric_limits<Ty>::epsilon() };
        auto dis{ Fabs(epsilon_num - num) };

        return (dis <= Epsilon);
    }

public:
    /// <summary>
    /// 一定の速度
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty Linear(Ty time, Ty totaltime, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        return (max - min) * time / totaltime + min;
    }

    /// <summary>
    /// 最も緩やか。
    /// 最初が緩やかで、徐々に速くなる
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty InSine(Ty time, Ty totaltime, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        max -= min;

        return -max * std::cos(time * ToRadian(_90<Ty>) / totaltime) + max + min;
    }

    /// <summary>
    /// 最も緩やか。
    /// 最初が速く、徐々に緩やかになる
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty OutSine(Ty time, Ty totaltime, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        max -= min;

        return max * std::sin(time * ToRadian(_90<Ty>) / totaltime) + min;
    }

    /// <summary>
    /// 最も緩やか。
    /// 緩やかに始まり中間付近で加速し、再度緩やかになる
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty InOutSine(Ty time, Ty totaltime, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        max -= min;

        return -max / _2<Ty> *(std::cos(time * Pai<Ty> / totaltime) - 1) + min;
    }

    /// <summary>
    /// 最も現実に即した。
    /// 最初が緩やかで、徐々に速くなる
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty InQuad(Ty time, Ty totaltime, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        max -= min;
        time /= totaltime;

        return max * time * time + min;
    }

    /// <summary>
    /// 最も現実に即した。
    /// 最初が速く、徐々に緩やかになる
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty OutQuad(Ty time, Ty totaltime, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        max -= min;
        time /= totaltime;

        return -max * time * (time - _2<Ty>) + min;
    }

    /// <summary>
    /// 最も現実に即した。
    /// 緩やかに始まり中間付近で加速し、再度緩やかになる
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty InOutQuad(Ty time, Ty totaltime, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        max -= min;
        time /= totaltime;

        if (time / _2<Ty> < _1<Ty>)
            return max / _2<Ty> *time * time + min;

        --time;

        return -max * (time * (time - _2<Ty>) - _1<Ty>) + min;
    }

    /// <summary>
    /// Quadよりさらに緩急が急。
    /// 最初が緩やかで、徐々に速くなる
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty InCubic(Ty time, Ty totaltime, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        max -= min;
        time /= totaltime;

        return max * time * time * time + min;
    }

    /// <summary>
    /// Quadよりさらに緩急が急。
    /// 最初が速く、徐々に緩やかになる
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty OutCubic(Ty time, Ty totaltime, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        max -= min;
        time = time / totaltime - _1<Ty>;

        return max * (time * time * time + _1<Ty>) + min;
    }

    /// <summary>
    /// Quadよりさらに緩急が急。
    /// 緩やかに始まり中間付近で加速し、再度緩やかになる
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty InOutCubic(Ty time, Ty totaltime, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        max -= min;
        time /= totaltime;

        if (time / _2<Ty> < _1<Ty>)
            return max / _2<Ty> *time * time * time + min;

        time -= _2<Ty>;

        return max / _2<Ty> *(time * time * time + _2<Ty>) + min;
    }

    /// <summary>
    /// Quad、Cubicよりさらに緩急が急。
    /// 最初はかなりゆっくり値が増え、時間が進むにつれて大きく値が変わります。
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty InQuart(Ty time, Ty totaltime, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        max -= min;
        time /= totaltime;

        return max * time * time * time * time + min;
    }

    /// <summary>
    /// Quad、Cubicよりさらに緩急が急。
    /// 最初一気に値が増え、後半はかなりゆっくり。
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty OutQuart(Ty time, Ty totaltime, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        max -= min;
        time = time / totaltime - _1<Ty>;

        return -max * (time * time * time * time - _1<Ty>) + min;
    }

    /// <summary>
    /// Quad、Cubicよりさらに緩急が急。
    /// かなり緩やかに始まり、中盤で一気に値が増えた後再びかなり緩やかになります。
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty InOutQuart(Ty time, Ty totaltime, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        max -= min;
        time /= totaltime;

        if (time / _2<Ty> < _1<Ty>)
            return max / _2<Ty> *time * time * time * time + min;

        time -= _2<Ty>;

        return -max / _2<Ty> *(time * time * time * time - _2<Ty>) + min;
    }

    /// <summary>
    /// Quad、Cubic、Quartよりさらに緩急が急。
    /// 最初は非常にゆっくり値が大きくなり、中盤から急激に値が大きくなる。
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty InQuint(Ty time, Ty totaltime, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        max -= min;
        time /= totaltime;

        return max * time * time * time * time * time + min;
    }

    /// <summary>
    /// Quad、Cubic、Quartよりさらに緩急が急。
    /// 最初に急激に値が大きくなり、後半からは非常にゆっくり大きくなる。
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty OutQuint(Ty time, Ty totaltime, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        max -= min;
        time = time / totaltime - _1<Ty>;

        return max * (time * time * time * time * time + _1<Ty>) + min;
    }

    /// <summary>
    /// Quad、Cubic、Quartよりさらに緩急が急。
    /// 非常にゆっくりと始まり、中盤で急激に値が大きくなり、再び終盤に向けて非常にゆっくりになります。
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty InOutQuint(Ty time, Ty totaltime, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        max -= min;
        time /= totaltime;

        if (time / _2<Ty> < _1<Ty>)
            return max / _2<Ty> *time * time * time * time * time + min;

        time -= _2<Ty>;

        return max / _2<Ty> *(time * time * time * time * time + _2<Ty>) + min;
    }

    /// <summary>
    /// Quint以上に急激な変化。
    /// 前半ほとんど変化せず、中盤に急に値が大きくなり始め、終盤は一気に大きくなる
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty InExp(Ty time, Ty totaltime, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        max -= min;

        return  AdjEqual<Ty>(time, _0<Ty>) ?
            min :
            max * std::pow(_2<Ty>, _10<Ty> *(time / totaltime - _1<Ty>)) + min;
    }

    /// <summary>
    /// Quint以上に急激な変化。
    /// 前半急激に値が大きくなり、中盤徐々に変化が緩やかになり、終盤はほとんど変化しない
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty OutExp(Ty time, Ty totaltime, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        max -= min;

        return AdjEqual<Ty>(time, totaltime) ?
            max + min :
            max * (-std::pow(_2<Ty>, -_10<Ty> *time / totaltime) + _1<Ty>) + min;
    }

    /// <summary>
    /// Quint以上に急激な変化。
    /// 前半はほとんど変わらず、中盤に急激に値が大きくなり、終盤再びほとんど変わらない
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty InOutExp(Ty time, Ty totaltime, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        if (AdjEqual<Ty>(time, _0<Ty>))
            return min;

        if (AdjEqual<Ty>(time, totaltime))
            return max;

        max -= min;
        time /= totaltime;

        if (time / _2<Ty> < _1<Ty>)
            return max / _2<Ty> *std::pow(_2<Ty>, _10<Ty> *(time - _1<Ty>)) + min;

        --time;

        return max / _2<Ty> *(-std::pow(_2<Ty>, -_10<Ty> *time) + _2<Ty>) + min;
    }

    /// <summary>
    /// 円周の4分の1を描くように進みかなり使い所に困る。
    /// 徐々に値の変化が大きくなっていき、最後の最後に一瞬で変化する
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty InCirc(Ty time, Ty totaltime, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        max -= min;
        time /= totaltime;
        time = (std::max)((std::min)(time, _1<Ty>), -_1<Ty>);


        return -max * (std::sqrt(_1<Ty> -time * time) - _1<Ty>) + min;
    }

    /// <summary>
    /// 円周の4分の1を描くように進みかなり使い所に困る。
    /// 最初に瞬間的に値が大きくなり、そのあとは徐々に変化量が小さくなる
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty OutCirc(Ty time, Ty totaltime, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        max -= min;
        time /= (totaltime - _1<Ty>);
        time = (std::max)((std::min)(time, _1<Ty>), -_1<Ty>);


        return max * std::sqrt(_1<Ty> -time * time) + min;
    }

    /// <summary>
    /// 円周の4分の1を描くように進みかなり使い所に困る。
    /// 最初は徐々に値が大きくなっていきますが、中盤一瞬で値が増え、終盤は再び緩やかに値が大きくなる
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty InOutCirc(Ty time, Ty totaltime, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        max -= min;
        time /= totaltime;
        time = (std::max)((std::min)(time, _1<Ty>), -_1<Ty>);


        if (time / _2<Ty> < _1<Ty>)
            return -max / _2<Ty> *(std::sqrt(_1<Ty> -time * time) - _1<Ty>) + min;

        time -= _2<Ty>;

        return max / _2<Ty> *(std::sqrt(_1<Ty> -time * time) + _1<Ty>) + min;
    }

    /// <summary>
    /// 簡単にアニメーションにアクセントをつけることができる。
    /// 最初に目的値と逆方向に変化し、その後目的値に向かって徐々に加速しながら進む
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty InBack(Ty time, Ty totaltime, Ty back, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        max -= min;
        time /= totaltime;

        return max * time * time * ((back + _1<Ty>) * time - back) + min;
    }

    /// <summary>
    /// 簡単にアニメーションにアクセントをつけることができる。
    /// 最初徐々に値が増えていき、目的値を過ぎてから徐々に減少して目的値に到達する
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty OutBack(Ty time, Ty totaltime, Ty back, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        max -= min;
        time = time / totaltime - _1<Ty>;

        return max * (time * time * ((back + _1<Ty>) * time * back) + _1<Ty>) + min;
    }

    /// <summary>
    /// 簡単にアニメーションにアクセントをつけることができる。
    /// 最初に目的値と逆側に向かってから目的値方向に進み、一度通り過ぎてから目的値に到達する
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty InOutBack(Ty time, Ty totaltime, Ty back, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        max -= min;
        back *= _1_525<Ty>;

        if (time / _2<Ty> < _1<Ty>)
            return max * (time * time * ((back + _1<Ty>) * time - back)) + min;

        time -= _2<Ty>;

        return max / _2<Ty> *(time * time * ((back + _1<Ty>) * time + back) + _2<Ty>) + min;
    }
    /// <summary>
    /// バウンドするような動き。
    /// 徐々にバウンドが大きくなっていく
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty InBounce(Ty time, Ty totaltime, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        return max - OutBounce<Ty>(totaltime - time, totaltime, max - min, _0<Ty>) + min;
    }

    /// <summary>
    /// バウンドするような動き。
    /// 徐々にバウンドが小さくなっていく
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty OutBounce(Ty time, Ty totaltime, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        max -= min;
        time /= totaltime;

        if (time < _1<Ty> / _2_75<Ty>)
            return max * (_7_5625<Ty> *time * time) + min;

        else if (time < _2<Ty> / _2_75<Ty>)
        {
            time -= _1_5<Ty> / _2_75<Ty>;

            return max * (_7_5625<Ty> *time * time + _0_75<Ty>) + min;
        }
        else if (time < _2_5<Ty> / _2_75<Ty>)
        {
            time -= _2_25<Ty> / _2_75<Ty>;

            return max * (_7_5625<Ty> *time * time + _0_9375<Ty>) + min;
        }
        else
        {
            time -= _2_625<Ty> / _2_75<Ty>;

            return max * (_7_5625<Ty> *time * time + _0_984375<Ty>) + min;
        }
    }

    /// <summary>
    /// バウンドするような動き。
    /// 徐々にバウンドが大きくなっていき、中盤を過ぎると徐々にバウンドが小さくなっていく
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty InOutBounce(Ty time, Ty totaltime, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        if (time < totaltime / _2<Ty>)
        {
            return InBounce<Ty>(time * _2<Ty>, totaltime, max - min, max) * _0_5<Ty> +min;
        }
        else
        {
            return OutBounce<Ty>(time * _2<Ty> -totaltime, totaltime, max - min, _0<Ty>) * _0_5<Ty> +min + (max - min) * _0_5<Ty>;
        }
    }
};
