#include <iostream>
#include <vector>
#include <stdlib.h>

using namespace std;

class Board
{
	private:
		int r,n;
		vector <int> k;
		vector <int> board;
		int found_val,found_prog;
		int last_elem = 0;
		bool comp_win = false;
		bool is_algorythm3 = false;
    public:
        Board();
        virtual ~Board();
        void display(const std::vector<int>& v);
    	void print();
    	bool check_sequence();
    	bool is_running();
        int algorythm1();
    	int algorythm3();
    	int request_algorythm();
    	int request();
    	void insert_coin(int position);
};

//Konstruktor - rozpoczęcie rozgrywki
Board::Board()
{
    int count_2 = 0;
    int count_3 = 0;
    cout << "Podaj liczbę dopuszczalnych kolorów" <<endl;
    cin >> r;
    cout << "Podaj maksymalne długości ciągów o i-tym kolorze" <<endl;
    k.resize(r);
    for(int i = 0; i < r; i++){
        cin >> k[i];
        if(k[i] == 2){count_2++;}
        if(k[i] == 3){count_3++;}
    }
    cout << "Podaj maksymalną liczbę żetonów, które mogą zostać ułożone w linii" << endl;
    cin >> n;
    cout << "Podaj pierwszy żeton" <<endl;
    int c;
    cin >> c;
    board.push_back(c);
    if(count_2 + count_3 == r){is_algorythm3 = true; cout<<"we will use the third algorythm!"<<endl;}
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

//Kolorowanie tablicy
void Board :: display(const std::vector<int>& v)
{
    int seq_length = (k[found_val-1]-1)*found_prog;
    int seq_start = last_elem - seq_length;
    for(int i = 0; i < seq_start;i++){
        cout << v[i] << " ";
    }
     for(int i = 0; i <= seq_length; i++){
            if(i % found_prog == 0){
                printf("\033[1;47;35m%d\033[0m ",v[i+seq_start]);
            }else{
                cout << v[i+seq_start] << " ";
            }
    }
    for(int i = last_elem + 1; i < v.size(); i++){
        cout << v[i] << " ";
    }
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
    cout << "plansza: ";display(board);cout<< endl;
}

//Zwraca true jeżeli znaleziony został ciąg długości k_i dla i-tego koloru.
//Po odkomentowaniu można lepiej prześledzić działanie algorytmu.
bool Board::check_sequence(){
    int val,prev_val,counter;
    for(int progression = 1; progression < board.size(); progression++){
        //cout << "Postęp arytmetyczny: " << progression << endl;
        for(int i = 0; i < progression; i++){
            //cout << "Zliczanie rozpoczęło się od elementu na " << i << "-pozycji."<<endl;
            counter = 1;
            prev_val = board[i];
                for(int pos = progression + i; pos < board.size(); pos+=progression){
                    val = board[pos];
                    //cout << "Wykonało się dla poszycji: " << pos<<" Porównało " << prev_val << " z " << val << " ";
                    if(val == prev_val){
                        counter++;
                        //cout << "I zwiększył licznik. Teraz count wynosi: " <<counter << endl;
                    }else{
                        //cout << "I nie zwiększył licznika. licznik wrócił do wartości 1."<<endl;
                        counter = 1;
                    }
                    if(k[val-1] == counter){
                        found_val = val;
                        found_prog = progression;
                        last_elem = pos;
                        comp_win = true;
                        return true;
                    }
                    prev_val = val;
                }
            if(progression + i == board.size()-1){ break;} // żeby nie wykonywać zbędnych obliczeń
            }
        }
    return false;
}

//Zwraca true jeżeli zachodzi jeden z warunków konća rozgrywki
bool Board::is_running(){
    if (check_sequence() || board.size() >= n){
        return false;
    }else {
        return true;
    }
}

int Board::algorythm1(){
    for(int i = 0; i < board.size()-1; i++){
        if(board[i] != board[i+1]){
            return i+1;
        }
    }
    return board.size();
}

int Board::algorythm3(){
    int j_color = 0;
    for(int i = 0; i < k.size(); i++){
        if(k[i] == 3){
            j_color = i;
            break;
        }
    }
    for(int i = 0; i < board.size(); i++){
        if(board[i] - 1 == j_color){
            return i;
        }
    }
    return board.size();
}
//TODO: Algorytm znajdujący optymalny ruch komputera. (póki co ruch komputera jest losowy)
int Board::request_algorythm(){
    if(r == 2) return algorythm1();
    if(is_algorythm3) return algorythm3();
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
