
#ifdef TEST_STATUS

#define LEAK_CHECK
#ifdef LEAK_CHECK
	#include <vld.h>
#endif

#include <string>
#include <sstream>
#include <iostream>
#include <functional>
#include <random>
#include <chrono>

#include "Status.h"

Status Func(bool pass) {
	if (pass) {
		return Status::Success();
	} else {
		return Status::Error("This is a failed function");
	}
}

class BingoStat : public Stat {
public:
	static const short draw_failure = -2;
	static const short card_failure = -1;
	static const short winning		=  0;
	static const short non_winning  =  1;

	const std::string card;
	const std::string draw;
	const int		  prize;

	BingoStat(short _type, int _prize, const std::string& _card, const std::string& _draw) :
		Stat(_type),
		prize(_prize),
		card(_card),
		draw(_draw) {}

	~BingoStat() {}

	static Status DrawFailed(const std::string& draw) {
		return Status(new BingoStat(draw_failure, 0, "", draw));
	}

	static Status CardFailed(const std::string& card) {
		return Status(new BingoStat(card_failure, 0, card, ""));
	}

	static Status Winning(int prize, const std::string& card, const std::string& draw) {
		return Status(new BingoStat(winning, prize, card, draw));
	}

	static Status NonWinning(const std::string& card, const std::string& draw) {
		return Status(new BingoStat(non_winning, 0, card, draw));
	}

	virtual const std::string Type() const {

		switch (type) {
		case draw_failure:
			return "===================\nDraw Failed";
		case card_failure:
			return "===================\nCard Failed";
		case winning:
			return "===================\nPrize Won";
		case non_winning:
			return "===================\nNo Prize";
		}

		return "===================\nUnknown BingoStatus";
	}

	virtual const std::string Desc() const {
		std::stringstream ss;
		ss << std::endl;

		switch (type) {
		case draw_failure:
			ss << "Draw: " << draw << std::endl;
			break;
		case card_failure:
			ss << "Card: " << card << std::endl;
			break;
		case winning:
			ss << "Prize: " << prize << std::endl;
			ss << "Card: " << card << std::endl;
			ss << "Draw: " << draw << std::endl;
			break;
		case non_winning:
			ss << "Card: " << card << std::endl;
			ss << "Draw: " << draw << std::endl;
			break;
		}

		ss << "===================\n";
		return ss.str();
	}
};

#define TIME_SEED (unsigned int)std::chrono::system_clock::now().time_since_epoch().count()

Status GenCard(std::string& _card) {
	std::mt19937 gen(TIME_SEED);
	std::uniform_int_distribution<int> dis(1, 35);

	std::stringstream ss;

	for (int i = 0; i < 10; ++i) {
		int spot = dis(gen);
		if (spot > 33) {
			return BingoStat::CardFailed(ss.str());
		}

		ss << spot << ", ";
	}

	_card = ss.str();

	return Status::Success();
}

Status DrawBalls(std::string& _draw) {
	std::mt19937 gen(TIME_SEED);
	std::uniform_int_distribution<int> dis(1, 35);

	std::stringstream ss;

	for (int i = 0; i < 15; ++i) {
		int ball = dis(gen);
		if (ball > 30) {
			return BingoStat::DrawFailed(ss.str());
		}

		ss << ball << ", ";
	}

	_draw = ss.str();

	return Status::Success();
}

Status PlayBingo() {
	std::string card;
	std::string draw;

	Status cardStat = GenCard(card);
	if (!cardStat) {
		return cardStat;
	}

	Status drawStat = DrawBalls(draw);
	if (!drawStat) {
		return drawStat;
	}

	std::mt19937 generator(TIME_SEED);
	std::uniform_int_distribution<int> winning(1, 10);
	std::uniform_int_distribution<int> prize(100, 1000);
	
	bool won = (winning(generator) >= 8);
	
	if (won) {
		int award = prize(generator);
		return BingoStat::Winning(award, card, draw);
	} else {
		return BingoStat::NonWinning(card, draw);
	}
}

int main(int argc, char** argv) {
	system("PAUSE");

	Status pass = Func(true);
	if (pass) {
		std::cout << pass << std::endl;
	}

	Status fail = Func(false);
	if (!fail) {
		std::cout << fail << std::endl;
	}

	for (int i = 0; i < 100; ++i) {
		Status game = PlayBingo();
		if (!game) {
			std::cerr << "Error Detected!" << std::endl;
			std::cerr << game;
		} else {
			std::cout << game;
		}
	}

	system("PAUSE");
	return 0;
}

#endif //TEST_STATUS