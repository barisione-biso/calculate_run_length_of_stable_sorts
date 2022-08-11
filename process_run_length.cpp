#include <iostream>
#include <fstream>
#include <deque>
#include <vector>
#include <sstream>
#include <algorithm>
// include input and output archivers
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

// include this header to serialize vectors
#include <boost/serialization/vector.hpp>

//stable_sort with dinamic content is slow for the test data set.
using row = std::deque<uint64_t>;
using matrix = std::vector<row>;

/*
Usando wikidata-filtered-enumerated.dat esta clase genera 3 archivos, [0,1,2].dat. los que se deben pasar con input a 'run_length_check'.
Este proceso es muy poco eficiente porque usa deque + vector. Debe haber otra manera de hacer el move to front sin deque.
TODO: representar de manera compacta usando int_vector o bit_vector de SDSL.
*/
class process_run_length{
    private:
        uint64_t i;
        char delim;
        int num_of_rows;
        int num_of_columns;
        matrix table;
    public:
        process_run_length(std::string input_file): num_of_rows(0), delim(' '){
            uint64_t processed = 0;
            std::ifstream ifs(input_file);
            std::string str_line;
            //matrix table;
            table.reserve(81426573);//MAX SIZE de wikidata-enumerated.dat
            while (std::getline(ifs, str_line)){
                std::deque<uint64_t> aux;
                // construct a stream from the string
                std::stringstream ss(str_line);
                std::string s;
                int col_idx = 1;
                while (std::getline(ss, s, delim)) {
                    int i = std::stoi(s);
                    aux.push_back((uint64_t) i);
                }
                table.push_back(aux);
                processed++;
                if(processed % 10000000 == 0)
                    std::cout << processed << " Processed rows." << std::endl;
            }
            ifs.close();
            table.shrink_to_fit();
            num_of_rows = table.size();
            num_of_columns = table[0].size();
            std::cout << "--Loaded " << num_of_rows << " rows" << " with " << table[0].size() << " columns each. " << std::endl;
        }
        void move_last_column_to_front(){
            matrix::iterator it, end = table.end();
            int i;
            for (i = 0, it = table.begin(); it != end; it++, i++){
                row &r = table[i];
                r.push_front(r[r.size() - 1]);
                r.pop_back();
            }
        }
        void pop_first_column(){
            matrix::iterator it, end = table.end();
            int i;
            for (i = 0, it = table.begin(); it != end; it++, i++){
                table[i].pop_front();
            }
        }
        /**
         * get_last_column_as_vector retrieves a std::vector<uint64_t> corresponding to the last column of table.
         * */
        std::vector<uint64_t> get_last_column_as_vector(){
            std::vector<uint64_t> l;
            l.reserve(81426573);
            int i;
            uint64_t num_rows = table.size();
            int last_column_id = table[0].size() - 1;
            for (i = 0; i < num_rows; i++){
                uint64_t number = table[i][last_column_id];
                l.push_back(number);
            }
            l.shrink_to_fit();
            /*uint64_t aux = l[0];
            int num_of_runs = 1;
            for (i = 1; i < l.size(); i++){
                if(aux != l[i]){
                    num_of_runs++;
                }
                aux = l[i];
            }
            std::cout << "Num_of_runs on Last Column: " << num_of_runs << std::endl;
            */
            return l;
        }
        void print_num_of_runs(int column_id){
            int num_of_runs = 1;
            uint64_t num_rows = table.size();
            uint64_t aux = table[0][column_id];
            for (uint64_t i = 0; i < num_rows; i++){
                auto value = table[i][column_id];
                if(aux != value){
                    num_of_runs++;
                }
                aux = value;
            }
            std::cout << "Num_of_runs on column "<< column_id << ": " << num_of_runs << std::endl;
        }
        void sort_table (){
            matrix::iterator it, table_begin = table.begin(), triple_end = table.end();
            std::sort(table_begin, triple_end);
        }

        void process_columns () {
            /*********************** PART 2.2 : Process k-1 ..+ 1 column ***********************/
            for(int i = 0; i < num_of_columns; i++){
                std::cout << "Order # " << (i+1) << std::endl << "========" << std::endl;
                if(i > 0){
                    move_last_column_to_front();
                }
                sort_table();
                //pop_first_column(table);
                //Print the number of runs of columns [0, |L| - 1].
                for(int i=0; i < table[0].size() - 1; i++){
                    print_num_of_runs(i);
                }
                const std::vector<uint64_t>& L = get_last_column_as_vector();
                //Print the number of runs of the last column.
                int last_column_id = table[0].size() - 1;
                print_num_of_runs(last_column_id);
                std::ofstream sof(  std::to_string(i)+".dat" );
                boost::archive::text_oarchive oarch(sof);
                // Save the data
                oarch &L;
            }
        }
};

int main(int argc, char **argv){
    if(argc != 2){
        std::cout << "Usage: " << argv[0] << "<dataset>" << std::endl;
        return 0;
    }
    std::string file= argv[1];
    /*********************** PART 1 : Read input file ***********************/
    process_run_length p = process_run_length(file);
    /*********************** PART 2 : Process columns (k) ***********************/
    p.process_columns();

    return 0;
}
