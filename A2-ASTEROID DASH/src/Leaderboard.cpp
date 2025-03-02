#include "Leaderboard.h"

// Read the stored leaderboard status from the given file such that the "head_leaderboard_entry" member
// variable will point to the highest all-times score, and all other scores will be reachable from it
// via the "next_leaderboard_entry" member variable pointer.
void Leaderboard::read_from_file(const string &filename) {
    std::ifstream file(filename);
    std::string line;
    if(file.is_open()) {
        std::getline (file, line);
        if(!line.empty()) {
            std::istringstream iss(line);
            string player_name;
            unsigned long score;
            time_t time_stamp;
            iss >> score >> time_stamp>> player_name;
            LeaderboardEntry* new_entry = new LeaderboardEntry( score, time_stamp, player_name);
            head_leaderboard_entry = new_entry;
        }
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            string player_name;
            unsigned long score;
            time_t time_stamp;
            iss >> score >> time_stamp>> player_name;
            LeaderboardEntry* new_entry = new LeaderboardEntry( score, time_stamp, player_name);
            head_leaderboard_entry->next= new_entry;
        }
    }
}


// Write the latest leaderboard status to the given file in the format specified in the PA instructions
void Leaderboard::write_to_file(const string &filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        LeaderboardEntry* current = head_leaderboard_entry;
        while (current != nullptr) {
            file << current->score << " " << current->last_played << " " << current->player_name << std::endl;
            current = current->next;
        }
        file.close();
    }
}

// Print the current leaderboard status to the standard output in the format specified in the PA instructions
void Leaderboard::print_leaderboard() {
    cout<<"Leaderboard"<<endl<<"-----------"<<endl;
    LeaderboardEntry* temp = head_leaderboard_entry;
    int count=0;
    while(temp!=nullptr){
        count++;
        char time_stampt[20];
        std::strftime(time_stampt, sizeof(time_stampt), "%H:%M:%S/%d.%m.%Y", std::localtime(&temp->last_played));
        cout<<count<<". "<<temp->player_name<<" "<<temp->score<<" "<<time_stampt<<endl;
        temp = temp->next;
    }
}

//  Insert a new LeaderboardEntry instance into the leaderboard, such that the order of the high-scores
//  is maintained, and the leaderboard size does not exceed 10 entries at any given time (only the
//  top 10 all-time high-scores should be kept in descending order by the score).
void Leaderboard::insert(LeaderboardEntry *new_entry) {
    if(head_leaderboard_entry == nullptr){
        head_leaderboard_entry = new_entry;
        return;
    }
    LeaderboardEntry* current = head_leaderboard_entry;
    LeaderboardEntry* previous = nullptr;
    while(current != nullptr && new_entry->score <= current->score) {
        previous = current;
        current = current->next;
    }
    if(previous!=nullptr) {
        previous->next = new_entry;
        new_entry->next = current;
    }
    else {
        head_leaderboard_entry = new_entry;
        new_entry->next = current;
    }
    int count= 0;
    LeaderboardEntry* temp = head_leaderboard_entry;
    LeaderboardEntry* previous2 = nullptr;
    while(temp!=nullptr){
        count++;
        previous2 = temp;
        temp = temp->next;
        if(count==MAX_LEADERBOARD_SIZE+1) {
            while(temp!=nullptr){
                LeaderboardEntry* temp2 = temp;
                temp = temp->next;
                delete temp2;
            }
            previous2->next = nullptr;
        }
    }
}

// Free dynamically allocated memory used for storing leaderboard entries
Leaderboard::~Leaderboard() {
    LeaderboardEntry* current = head_leaderboard_entry;
    while (current != nullptr) {
        LeaderboardEntry* temp = current;
        current = current->next;
        delete temp;
    }
}
