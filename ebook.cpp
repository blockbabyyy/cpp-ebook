#include <algorithm>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <vector>
#include <string>


static const int MAXIMUM_USER_ID = 1e+5 + 1;
static const size_t MAXIMUM_PAGE_NUMBER = 1000;

struct Request {
    std::string type;
    int id;
    int page_num;
};



class Ebook {

public:

    Ebook() : personal_stat_(MAXIMUM_USER_ID, 0), global_stat_(MAXIMUM_PAGE_NUMBER, 0) {}

    void Read(int id, int page) {

        size_t pages_before = 0;

        if (personal_stat_[id]) {
            pages_before = personal_stat_[id];
            global_stat_[pages_before] -= 1;
        }
        else {
            ++users_count_;
        }

        personal_stat_[id] = page;
        global_stat_[page] += 1;
        

    }

    double Cheer(int id) const {
        

        if (!personal_stat_[id]) {
            return 0.0;
        }

        if (users_count_ == 1){
        	return 1.0;
		}

        int current_page_num = personal_stat_.at(id);

        
        int users_before_current_page = std::accumulate(global_stat_.begin(), global_stat_.begin() + current_page_num, 0);
        return static_cast<double>(users_before_current_page) / (users_count_ - 1);
    }
private:
    std::vector<int> personal_stat_;
    std::vector<int> global_stat_;
    int users_count_ = 0;

    
};


std::vector<Request> ParseRequests(std::istream& input, int count) {
    std::vector<Request> requests;

    for (int i = 0; i < count; ++i ) {

	    std::string type, id, page;
	    getline(input, type, ' ');

	    if (type == "CHEER") {
	        getline(input, id);
			requests.push_back({ type, std::stoi(id), 0 });
		}
	    else {
	        getline(input, id, ' ');
	        getline(input, page);
	        requests.push_back({ type, std::stoi(id), std::stoi(page) });
	    }
    }
    return requests;
}

void ReturnAnswers(std::istream& input, std::ostream& out, Ebook& ebook) {
    int request_count = 0;
    std::string request_count_str;
    getline(input, request_count_str);
    request_count = std::stoi(std::move(request_count_str));


    for (const auto& request : ParseRequests(input, request_count)) {
        
        if (request.type == "READ") {
            ebook.Read(request.id, request.page_num);
        }
        else if (request.type == "CHEER") {
            out << std::setprecision(6) << ebook.Cheer(request.id) << std::endl;
        }
        else {
            throw std::invalid_argument("Wrong request!");
        }
    }
}

int main() {
    Ebook ebook;
    ReturnAnswers(std::cin, std::cout, ebook);
    int i = 0;
}