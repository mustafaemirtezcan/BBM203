#include "GameController.h"
// Simply instantiates the game
GameController::GameController(
        const string &space_grid_file_name,
        const string &celestial_objects_file_name,
        const string &leaderboard_file_name,
        const string &player_file_name,
        const string &player_name

) {
    game = new AsteroidDash(space_grid_file_name, celestial_objects_file_name, leaderboard_file_name, player_file_name,
                            player_name);
}

// Reads commands from the given input file, executes each command in a game tick
void GameController::play(const string &commands_file) {
    std::ifstream file(commands_file);
    std::string line;
    if(file.is_open()) {
        while (std::getline(file, line)) {
            int temp=game->player->lives;
            if (line == "MOVE_UP") {
                game->player->move_up();
                game->update_space_grid();
            } else if (line == "MOVE_DOWN") {
                game->player->move_down(game->space_grid.size());
                game->update_space_grid();
            } else if (line == "MOVE_LEFT") {
                game->player->move_left();
                game->update_space_grid();
            } else if (line == "MOVE_RIGHT") {
                game->player->move_right(game->space_grid[0].size());
                game->update_space_grid();
            } else if (line == "SHOOT") {
                game->shoot();
                game->update_space_grid();
            }
            else if(line =="PRINT_GRID") {
                game->update_space_grid();
                game->print_space_grid();
            }else if(line=="NOP") {
                game->update_space_grid();
            }
            else {
                cout<< "Unknown command: "<<line<<endl;
                game->current_score--;
                game->game_time--;
            }
            if(game->player->lives==temp-1) {
                game->current_score--;
            }
            game->current_score++;
            if(game->game_over) {
                break;
            }
            game->game_time++;
        }
        time_t current_time = std::time(nullptr);
        LeaderboardEntry* new_entry= new LeaderboardEntry( game->current_score,current_time,game->player->player_name);
        game->leaderboard.insert(new_entry);
        game->leaderboard.write_to_file(game->leaderboard_file_name);
        if(game->game_over){cout<<"GAME OVER!"<<endl<<"Tick: "<<game->game_time<<endl<<"Lives: "<<game->player->lives<<endl<<"Ammo: "<<game->player->current_ammo<<endl<<"Score: "<<game->current_score<<endl<<"High Score: "<<game->leaderboard.head_leaderboard_entry->score<<endl<<"Player: "<<game->player->player_name<<endl;}else {
            cout<<"GAME FINISHED! No more commands!"<<endl<<"Tick: "<<game->game_time-1<<endl<<"Lives: "<<game->player->lives<<endl<<"Ammo: "<<game->player->current_ammo<<endl<<"Score: "<<game->current_score<<endl<<"High Score: "<<game->leaderboard.head_leaderboard_entry->score<<endl<<"Player: "<<game->player->player_name<<endl;
        }
        for (const auto& row : game->space_grid) {
            for (int cell : row) {
                if (cell == 1) {
                    cout << occupiedCellChar;
                } else {
                    cout << unoccupiedCellChar;
                }
            }
            cout << endl;
        }
        cout << endl;
        game->leaderboard.print_leaderboard();
    }
}

// Destructor to delete dynamically allocated member variables here
GameController::~GameController() {
    delete game;
}
