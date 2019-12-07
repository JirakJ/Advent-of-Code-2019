#include <utility>

#include <iostream>
#include <chrono>
#include <utility>
#include <vector>
#include <algorithm>
#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <cstring>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <iterator>

class HelperIntCode
{
public:
    explicit HelperIntCode(const std::vector<int>& pg) :
            mPg{pg}
    {}

    static HelperIntCode HandleInput(std::vector<int> vect)
    {
        return HelperIntCode{vect};
    }

    void Reset(const HelperIntCode& p)
    {
        if (p.mPg.size() != mPg.size()) throw 42;
        std::memcpy(mPg.data(), p.mPg.data(), p.mPg.size());
        mIp = 0;
    }

    template <class OnRead, class OnWrite>
    void Execute(OnRead onRead, OnWrite onWrite)
    {
        while (ExecuteInstruction(onRead, onWrite));
    }

    enum struct Opcode {
        Add = 1,
        Mul = 2,
        Read = 3,
        Write = 4,
        JNZ = 5,
        JZ = 6,
        LT = 7,
        EQ = 8,
        End = 99
    };
    enum struct Mode {
        Position = 0,
        Immediate = 1
    };

    int Read(Mode mode, size_t mIp)
    {
        if(mode==Mode::Position)
            return mPg.at(static_cast<unsigned long>(mPg[mIp]));
        if(mode==Mode::Immediate)
            return mPg[mIp];
    }

    template <class OnRead, class OnWrite>
    bool ExecuteInstruction(OnRead onRead, OnWrite onWrite)
    {
        const std::array<Mode, 3> m{Mode{(mPg[mIp] / 100) % 10}, Mode{(mPg[mIp] / 1000) % 10}, Mode{(mPg[mIp] / 10000) % 10}};
        const Opcode opcode{mPg[mIp] % 100};

        switch (opcode)
        {
            case Opcode::End:
                return false;

            case Opcode::Add:
            case Opcode::Mul:
            {
                const int op1 = Read(m[0], static_cast<size_t>(mIp + 1));
                const int op2 = Read(m[1], static_cast<size_t>(mIp + 2));
                if (m[2] != Mode::Position) throw std::runtime_error("unexpected mode");
                mPg.at(static_cast<size_t>(mPg[mIp + 3])) = opcode == Opcode::Add ? op1 + op2 : op1 * op2;
                mIp += 4;
                break;
            }

            case Opcode::Read:
            {
                int value = onRead();
                if (m[0] != Mode::Position) throw std::runtime_error("unexpected mode");
                mPg.at(static_cast<size_t>(mPg[mIp + 1])) = value;
                mIp += 2;
                break;
            }

            case Opcode::Write:
                onWrite(Read(m[0], mIp + 1));
                mIp += 2;
                break;

            case Opcode::JNZ:
                if (Read(m[0], mIp + 1) != 0)
                    mIp = static_cast<size_t>(Read(m[1], mIp + 2));
                else
                    mIp += 3;
                break;

            case Opcode::JZ:
                if (Read(m[0], mIp + 1) == 0)
                    mIp = static_cast<size_t>(Read(m[1], mIp + 2));
                else
                    mIp += 3;
                break;

            case Opcode::LT:
                if (m[2] != Mode::Position) throw std::runtime_error("unexpected mode");
                mPg.at(static_cast<size_t>(mPg[mIp + 3])) = Read(m[0], mIp + 1) < Read(m[1], mIp + 2) ? 1 : 0;
                mIp += 4;
                break;

            case Opcode::EQ:
                if (m[2] != Mode::Position) throw std::runtime_error("unexpected mode");
                mPg.at(static_cast<size_t>(mPg[mIp + 3])) = Read(m[0], mIp + 1) == Read(m[1], mIp + 2) ? 1 : 0;mIp += 4;
                break;
        }
        return true;
    }

private:
    std::vector<int> mPg;
    std::size_t mIp;
};

struct day7
{
	int part1(std::vector<int> vect)
	{
		HelperIntCode helperIntCode = HelperIntCode::HandleInput(std::move(vect));
		std::array<int, 5> phases{0, 1, 2, 3, 4};
		std::array<HelperIntCode, 5> amps{helperIntCode, helperIntCode, helperIntCode, helperIntCode, helperIntCode};
		int maxThurster = 0;

		do
		{
			int output = 0;

			for (int i = 0; i < 5; ++i)
				amps[i].Reset(helperIntCode);

			for (int i = 0; i < 5; ++i)
			{
				int read = 0;
				amps[i].Execute([&]() { return read++ == 0 ? phases[i] : output; },
								[&](int x) { output = x; });
			}

			maxThurster = std::max(maxThurster, output);
		} while (std::next_permutation(phases.begin(), phases.end()));
		return maxThurster;
	}

	int part2(std::vector<int> vect)
	{
		HelperIntCode acs = HelperIntCode::HandleInput(std::move(vect));
		std::array<HelperIntCode, 5> amps{acs, acs, acs, acs, acs};
		std::array<int, 5> phases{5, 6, 7, 8, 9};
		int maxThurster = 0;

		do
		{
			for (int i = 0; i < 5; ++i)
				amps[i].Reset(acs);

			std::array<int, 5> reads{0, 0, 0, 0, 0};
			std::array<int, 5> writes{0, 0, 0, 0, 0};
			std::array<bool, 5> running{true, true, true, true, true};
			int output = 0;

			for (int run = 0; std::any_of(running.cbegin(), running.cend(), [](bool b) { return b; }); ++run)
			{
				for (int i = 0; i < 5; ++i)
				{
					while (running[i] && writes[i] == run)
					{
						running[i] &= amps[i].ExecuteInstruction(
									[&]() { return reads[i]++ == 0 ? phases[i] : output; },
									[&](int x) { ++writes[i]; output = x; });
					}
				}
			}

			maxThurster = std::max(maxThurster, output);
		} while (std::next_permutation(phases.begin(), phases.end()));
		return maxThurster;
	}
};

void getPart1(std::vector<int> vect);
void getPart2(std::vector<int> vect);

void getPart1(std::vector<int> vect) {
    auto startTs = std::__1::chrono::steady_clock::now();

    day7 d;

    const int s = d.part1(vect);

    const auto endTs = std::__1::chrono::steady_clock::now();
    std::__1::cout << "Part 1: " << s << " and it took " << std::chrono::duration_cast<std::__1::chrono::microseconds>(endTs - startTs).count() << "us" << std::__1::endl;
}

void getPart2(std::vector<int> vect) {
    auto startTs = std::__1::chrono::steady_clock::now();

    day7 d;

    const int s = d.part2(vect);

    const auto endTs = std::__1::chrono::steady_clock::now();
    std::__1::cout << "Part 2: " << s << " and it took " << std::chrono::duration_cast<std::__1::chrono::microseconds>(endTs - startTs).count() << "us" << std::__1::endl;
}

int main()
{
    std::vector<int> vect{3,8,1001,8,10,8,105,1,0,0,21,34,59,76,101,114,195,276,357,438,99999,3,9,1001,9,4,9,1002,9,4,9,4,9,99,3,9,102,4,9,9,101,2,9,9,102,4,9,9,1001,9,3,9,102,2,9,9,4,9,99,3,9,101,4,9,9,102,5,9,9,101,5,9,9,4,9,99,3,9,102,2,9,9,1001,9,4,9,102,4,9,9,1001,9,4,9,1002,9,3,9,4,9,99,3,9,101,2,9,9,1002,9,3,9,4,9,99,3,9,101,2,9,9,4,9,3,9,1002,9,2,9,4,9,3,9,1002,9,2,9,4,9,3,9,1001,9,1,9,4,9,3,9,102,2,9,9,4,9,3,9,101,1,9,9,4,9,3,9,1001,9,1,9,4,9,3,9,1002,9,2,9,4,9,3,9,1001,9,2,9,4,9,3,9,102,2,9,9,4,9,99,3,9,101,2,9,9,4,9,3,9,1002,9,2,9,4,9,3,9,1001,9,1,9,4,9,3,9,101,2,9,9,4,9,3,9,1002,9,2,9,4,9,3,9,102,2,9,9,4,9,3,9,101,1,9,9,4,9,3,9,1001,9,2,9,4,9,3,9,101,2,9,9,4,9,3,9,1001,9,1,9,4,9,99,3,9,102,2,9,9,4,9,3,9,1001,9,1,9,4,9,3,9,1001,9,1,9,4,9,3,9,102,2,9,9,4,9,3,9,1001,9,2,9,4,9,3,9,1002,9,2,9,4,9,3,9,102,2,9,9,4,9,3,9,102,2,9,9,4,9,3,9,101,1,9,9,4,9,3,9,101,2,9,9,4,9,99,3,9,1002,9,2,9,4,9,3,9,102,2,9,9,4,9,3,9,102,2,9,9,4,9,3,9,102,2,9,9,4,9,3,9,1002,9,2,9,4,9,3,9,1002,9,2,9,4,9,3,9,101,2,9,9,4,9,3,9,102,2,9,9,4,9,3,9,101,2,9,9,4,9,3,9,101,2,9,9,4,9,99,3,9,1002,9,2,9,4,9,3,9,1001,9,1,9,4,9,3,9,101,2,9,9,4,9,3,9,101,2,9,9,4,9,3,9,101,2,9,9,4,9,3,9,101,1,9,9,4,9,3,9,1002,9,2,9,4,9,3,9,1002,9,2,9,4,9,3,9,1001,9,1,9,4,9,3,9,101,2,9,9,4,9,99};

    getPart1(vect);
    getPart2(vect);

    return 0;
}