#include <iostream>
#include <vector>

using namespace std;

class Board
{
	private:
		int r,n;
		vector <int> k;
		vector <int> board;
		int found_val,found_prog;
		bool comp_win = false;
    public:
        Board();
        virtual ~Board();
    	void print();
    	bool check_sequence();
    	bool is_running();
    	int request_algorythm();
    	int request();
    	void insert_coin(int position);
};

//Konstruktor - rozpoczęcie rozgrywki
Board::Board()
{
    cout << "Podaj liczbę dopuszczalnych kolorów" <<endl;
    cin >> r;
    cout << "Podaj maksymalne długości ciągów o i-tym kolorze" <<endl;
    k.resize(r);
    for(int i = 0; i < r; i++){
        cin >> k[i];
    }
    cout << "Podaj maksymalną liczbę żetonów, które mogą zostać ułożone w linii" << endl;
    cin >> n;
    cout << "Podaj pierwszy żeton" <<endl;
    int c;
    cin >> c;
    board.push_back(c);
}

//Przeciążony operator << do wyświetlania tablicy
template < class T >
std::ostream& operator << (std::ostream& os, const std::vector<T>& v)
{
    for (typename std::vector<T>::const_iterator ii = v.begin(); ii != v.end(); ++ii)
    {
        os << *ii << " ";
    }
    return os;
}

//Wyświetlanie podsumowania rozgrywki
void Board::print(){
    cout << endl;
    cout << "------------------KONIEC ROZGRYWKI------------------"<<endl;
    if(comp_win){
        cout << "Wygrał komputer - Utowarzyłeś ciąg koloru " << found_val << " długości " << k[found_val-1] << " o postępie arytemtycznym " << found_prog << endl;
    }else{
        cout << "Wygrałeś. Długość ciągu osiągnełeła maksymalną wartość (" << n << ")"<<endl;
    }
    cout << "liczba kolorów: " << r << endl << "maksymalne długosci ciągów: " << k << endl;
    cout << "plansza: " << board<< endl;

}

//Zwraca true jeżeli znaleziony został ciąg długości k_i dla i-tego koloru
bool Board::check_sequence(){
    int val,prev_val,counter;
    for(int progression = 1; progression < board.size(); progression++){
            counter = 1;
        for(int i = 0; i <= progression; i++){
        prev_val = board[i];
            for(int pos = progression + i; pos < board.size(); pos+=progression){
                val = board[pos];
                if(val == prev_val){
                    counter++;
                }else{
                    counter = 1;
                }
                if(k[val-1] <= counter){
                    found_val = val;
                    found_prog = progression;
                    comp_win = true;
                    return true;
                }
                prev_val = val;
            }
        }
    }
    return false;
}

//Zwraca true jeżeli zachodzi jeden z warunków konća rozgrywki
bool Board::is_running(){
    if (board.size() >= n || check_sequence()){
        return false;
    }else {
        return true;
    }
}

//TODO: Algorytm znajdujący optymalny ruch komputera. (póki co ruch komputera jest losowy)
int Board::request_algorythm(){
    srand( (unsigned)time(NULL) );
    return rand() % board.size();
}

//Rządanie komputera o wstawienia kolejnego żetonu przez gracza
int Board::request(){
    int position = request_algorythm();
    cout << "Wstaw żeton na wskazanej pozycji (" << position + 1<< ") : " << endl;
    std::vector<int> split_lo(board.begin(), board.begin() + position);
    std::vector<int> split_hi(board.begin() + position, board.end());
    cout << split_lo << "_ " << split_hi <<endl;
    return position;
}

//Wstawia przez gracza żeton na wybranej pozycji
void Board::insert_coin(int position){
    int value;
    cin >> value;
    if(value < 0 || value > r){
        cerr <<  "Ten kolor nie jest dostępny" << endl;
    }else{
    board.insert(board.begin() + position, value);
    }
}

Board::~Board()
{

}

int main()
{
    Board B = Board();
    while(B.is_running()){
    int position = B.request();
    B.insert_coin(position);
    }
    B.print();
    return 0;
}