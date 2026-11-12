#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

// カードの情報
struct Card {
    int suit;       // マーク（0～3）
    int number;     // 数字（1～13）
};

// 52枚のカードを管理するクラス
class Shoe {
private:
    int cards[52];
    int remaining;

public:
    // コンストラクタ
    Shoe() {
        reset();
    }

    // カードを52枚に戻す
    void reset() {
        for (int i = 0; i < 52; i++) {
            cards[i] = i;
        }
        remaining = 52;
    }

    // カードを1枚引く
    Card takeCard() {
        // 52枚引き終わったらリセット
        if (remaining == 0) {
            reset();
        }

        // 残っているカードから選ぶ
        int index = rand() % remaining;
        int id = cards[index];

        // 引いたカードを残りの対象から外す
        cards[index] = cards[remaining - 1];
        remaining--;

        // カードの種類と数字を求める
        Card result;
        result.suit = id / 13;
        result.number = id % 13 + 1;

        return result;
    }
};

// プレイヤーとディーラーの親クラス
class Participant {
protected:
    string name;
    Card card;

public:
    // コンストラクタ
    Participant(const string& n) : name(n) {
        card.suit = -1;
        card.number = 0;
    }

    virtual ~Participant() {}

    // カードを引く共通処理
    void hit(Shoe* shoe) {
        card = shoe->takeCard();
    }

    // 名前を取得
    string getName() const {
        return name;
    }

    // カードの数字を取得
    int getNumber() const {
        return card.number;
    }

    // カードのマークを取得
    int getSuit() const {
        return card.suit;
    }

    // 役割の表示は子クラスで実装
    virtual void showRole() const = 0;
};

// プレイヤークラス
class Player : public Participant {
public:
    Player(const string& n) : Participant(n) {}

    void showRole() const override {
        cout << "Player" << endl;
    }
};

// ディーラークラス
class Dealer : public Participant {
public:
    Dealer(const string& n) : Participant(n) {}

    void showRole() const override {
        cout << "Dealer" << endl;
    }
};

// Participantのポインタを受け取る通常の関数
void showCardType(Participant* person) {
    const char* suits[4] = {
        "Spades", "Hearts", "Diamonds", "Clubs"
    };

    const char* ranks[13] = {
        "A", "2", "3", "4", "5", "6", "7",
        "8", "9", "10", "J", "Q", "K"
    };

    person->showRole();

    if (person->getNumber() == 0) {
        cout << "No card" << endl;
        return;
    }

    cout << person->getName() << ": "
        << suits[person->getSuit()] << " "
        << ranks[person->getNumber() - 1]
        << endl;
}

int main() {
    // 乱数の初期化
    srand(static_cast<unsigned int>(time(nullptr)));

    // オブジェクトを作成
    Shoe shoe;
    Player player("You");
    Dealer dealer("CPU");

    // それぞれカードを引く
    player.hit(&shoe);
    dealer.hit(&shoe);

    // カード情報を表示
    showCardType(&player);
    showCardType(&dealer);

    // 勝敗判定
    if (player.getNumber() > dealer.getNumber()) {
        cout << "You win!" << endl;
    }
    else if (player.getNumber() < dealer.getNumber()) {
        cout << "You lose!" << endl;
    }
    else {
        cout << "Draw!" << endl;
    }

    return 0;
}
