#pragma once
#include <cmath>
// �C�[�W���O�֐�
class Easing
{
private:
    template <class Ty = float>
    static constexpr Ty PaiF{ static_cast<Ty>(3.141592653589793f) }; // �~����

    template <class Ty = double>
    static constexpr Ty Pai{ static_cast<Ty>(3.141592653589793) }; // �~����

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
    /// ���̑��x
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty Linear(Ty time, Ty totaltime, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        return (max - min) * time / totaltime + min;
    }

    /// <summary>
    /// �ł��ɂ₩�B
    /// �ŏ����ɂ₩�ŁA���X�ɑ����Ȃ�
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty InSine(Ty time, Ty totaltime, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        max -= min;

        return -max * std::cos(time * ToRadian(_90<Ty>) / totaltime) + max + min;
    }

    /// <summary>
    /// �ł��ɂ₩�B
    /// �ŏ��������A���X�Ɋɂ₩�ɂȂ�
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty OutSine(Ty time, Ty totaltime, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        max -= min;

        return max * std::sin(time * ToRadian(_90<Ty>) / totaltime) + min;
    }

    /// <summary>
    /// �ł��ɂ₩�B
    /// �ɂ₩�Ɏn�܂蒆�ԕt�߂ŉ������A�ēx�ɂ₩�ɂȂ�
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty InOutSine(Ty time, Ty totaltime, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        max -= min;

        return -max / _2<Ty> *(std::cos(time * Pai<Ty> / totaltime) - 1) + min;
    }

    /// <summary>
    /// �ł������ɑ������B
    /// �ŏ����ɂ₩�ŁA���X�ɑ����Ȃ�
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty InQuad(Ty time, Ty totaltime, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        max -= min;
        time /= totaltime;

        return max * time * time + min;
    }

    /// <summary>
    /// �ł������ɑ������B
    /// �ŏ��������A���X�Ɋɂ₩�ɂȂ�
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty OutQuad(Ty time, Ty totaltime, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        max -= min;
        time /= totaltime;

        return -max * time * (time - _2<Ty>) + min;
    }

    /// <summary>
    /// �ł������ɑ������B
    /// �ɂ₩�Ɏn�܂蒆�ԕt�߂ŉ������A�ēx�ɂ₩�ɂȂ�
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
    /// Quad��肳��Ɋɋ}���}�B
    /// �ŏ����ɂ₩�ŁA���X�ɑ����Ȃ�
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty InCubic(Ty time, Ty totaltime, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        max -= min;
        time /= totaltime;

        return max * time * time * time + min;
    }

    /// <summary>
    /// Quad��肳��Ɋɋ}���}�B
    /// �ŏ��������A���X�Ɋɂ₩�ɂȂ�
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty OutCubic(Ty time, Ty totaltime, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        max -= min;
        time = time / totaltime - _1<Ty>;

        return max * (time * time * time + _1<Ty>) + min;
    }

    /// <summary>
    /// Quad��肳��Ɋɋ}���}�B
    /// �ɂ₩�Ɏn�܂蒆�ԕt�߂ŉ������A�ēx�ɂ₩�ɂȂ�
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
    /// Quad�ACubic��肳��Ɋɋ}���}�B
    /// �ŏ��͂��Ȃ�������l�������A���Ԃ��i�ނɂ�đ傫���l���ς��܂��B
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty InQuart(Ty time, Ty totaltime, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        max -= min;
        time /= totaltime;

        return max * time * time * time * time + min;
    }

    /// <summary>
    /// Quad�ACubic��肳��Ɋɋ}���}�B
    /// �ŏ���C�ɒl�������A�㔼�͂��Ȃ�������B
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty OutQuart(Ty time, Ty totaltime, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        max -= min;
        time = time / totaltime - _1<Ty>;

        return -max * (time * time * time * time - _1<Ty>) + min;
    }

    /// <summary>
    /// Quad�ACubic��肳��Ɋɋ}���}�B
    /// ���Ȃ�ɂ₩�Ɏn�܂�A���Ղň�C�ɒl����������Ăт��Ȃ�ɂ₩�ɂȂ�܂��B
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
    /// Quad�ACubic�AQuart��肳��Ɋɋ}���}�B
    /// �ŏ��͔��ɂ������l���傫���Ȃ�A���Ղ���}���ɒl���傫���Ȃ�B
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty InQuint(Ty time, Ty totaltime, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        max -= min;
        time /= totaltime;

        return max * time * time * time * time * time + min;
    }

    /// <summary>
    /// Quad�ACubic�AQuart��肳��Ɋɋ}���}�B
    /// �ŏ��ɋ}���ɒl���傫���Ȃ�A�㔼����͔��ɂ������傫���Ȃ�B
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty OutQuint(Ty time, Ty totaltime, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        max -= min;
        time = time / totaltime - _1<Ty>;

        return max * (time * time * time * time * time + _1<Ty>) + min;
    }

    /// <summary>
    /// Quad�ACubic�AQuart��肳��Ɋɋ}���}�B
    /// ���ɂ������Ǝn�܂�A���Ղŋ}���ɒl���傫���Ȃ�A�ĂяI�ՂɌ����Ĕ��ɂ������ɂȂ�܂��B
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
    /// Quint�ȏ�ɋ}���ȕω��B
    /// �O���قƂ�Ǖω������A���Ղɋ}�ɒl���傫���Ȃ�n�߁A�I�Ղ͈�C�ɑ傫���Ȃ�
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
    /// Quint�ȏ�ɋ}���ȕω��B
    /// �O���}���ɒl���傫���Ȃ�A���Տ��X�ɕω����ɂ₩�ɂȂ�A�I�Ղ͂قƂ�Ǖω����Ȃ�
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
    /// Quint�ȏ�ɋ}���ȕω��B
    /// �O���͂قƂ�Ǖς�炸�A���Ղɋ}���ɒl���傫���Ȃ�A�I�ՍĂтقƂ�Ǖς��Ȃ�
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
    /// �~����4����1��`���悤�ɐi�݂��Ȃ�g�����ɍ���B
    /// ���X�ɒl�̕ω����傫���Ȃ��Ă����A�Ō�̍Ō�Ɉ�u�ŕω�����
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
    /// �~����4����1��`���悤�ɐi�݂��Ȃ�g�����ɍ���B
    /// �ŏ��ɏu�ԓI�ɒl���傫���Ȃ�A���̂��Ƃ͏��X�ɕω��ʂ��������Ȃ�
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
    /// �~����4����1��`���悤�ɐi�݂��Ȃ�g�����ɍ���B
    /// �ŏ��͏��X�ɒl���傫���Ȃ��Ă����܂����A���Ո�u�Œl�������A�I�Ղ͍Ăъɂ₩�ɒl���傫���Ȃ�
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
    /// �ȒP�ɃA�j���[�V�����ɃA�N�Z���g�����邱�Ƃ��ł���B
    /// �ŏ��ɖړI�l�Ƌt�����ɕω����A���̌�ړI�l�Ɍ������ď��X�ɉ������Ȃ���i��
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty InBack(Ty time, Ty totaltime, Ty back, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        max -= min;
        time /= totaltime;

        return max * time * time * ((back + _1<Ty>) * time - back) + min;
    }

    /// <summary>
    /// �ȒP�ɃA�j���[�V�����ɃA�N�Z���g�����邱�Ƃ��ł���B
    /// �ŏ����X�ɒl�������Ă����A�ړI�l���߂��Ă��珙�X�Ɍ������ĖړI�l�ɓ��B����
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty OutBack(Ty time, Ty totaltime, Ty back, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        max -= min;
        time = time / totaltime - _1<Ty>;

        return max * (time * time * ((back + _1<Ty>) * time * back) + _1<Ty>) + min;
    }

    /// <summary>
    /// �ȒP�ɃA�j���[�V�����ɃA�N�Z���g�����邱�Ƃ��ł���B
    /// �ŏ��ɖړI�l�Ƌt���Ɍ������Ă���ړI�l�����ɐi�݁A��x�ʂ�߂��Ă���ړI�l�ɓ��B����
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
    /// �o�E���h����悤�ȓ����B
    /// ���X�Ƀo�E���h���傫���Ȃ��Ă���
    /// </summary>
    template<typename Ty = float>
    [[nodiscard]] static inline Ty InBounce(Ty time, Ty totaltime, Ty max = _1<Ty>, Ty min = _0<Ty>)
    {
        return max - OutBounce<Ty>(totaltime - time, totaltime, max - min, _0<Ty>) + min;
    }

    /// <summary>
    /// �o�E���h����悤�ȓ����B
    /// ���X�Ƀo�E���h���������Ȃ��Ă���
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
    /// �o�E���h����悤�ȓ����B
    /// ���X�Ƀo�E���h���傫���Ȃ��Ă����A���Ղ��߂���Ə��X�Ƀo�E���h���������Ȃ��Ă���
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
