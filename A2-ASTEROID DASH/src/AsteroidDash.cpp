#include "AsteroidDash.h"

std::vector<std::vector<int>> projectiles;
// Constructor to initialize AsteroidDash with the given parameters
AsteroidDash::AsteroidDash(const string &space_grid_file_name,
                           const string &celestial_objects_file_name,
                           const string &leaderboard_file_name,
                           const string &player_file_name,
                           const string &player_name)

    : leaderboard_file_name(leaderboard_file_name), leaderboard(Leaderboard()) {
    read_player(player_file_name, player_name); // Initialize player using the player.dat file
    read_space_grid(space_grid_file_name); // Initialize the grid after the player is loaded
    read_celestial_objects(celestial_objects_file_name); // Load celestial objects
    leaderboard.read_from_file(leaderboard_file_name);
}

// Function to read the space grid from a file
void AsteroidDash::read_space_grid(const string &input_file) {
    std::ifstream file(input_file);
    std::string line;
    while (std::getline(file, line)) {
        vector<int> row;
        for (char c: line) {
            if (c == '0') {
                row.push_back(0);
            } else if (c == '1') {
                row.push_back(1);
            }
        }
        space_grid.push_back(row);
    }
    file.close();
    // TODO: Your code here
}

// Function to read the player from a file
void AsteroidDash::read_player(const string &player_file_name, const string &player_name) {
    std::ifstream file(player_file_name);
    int row_position;
    int col_position;
    vector<vector<bool> > spacecraft_shape;
    std::string first_line;
    std::getline(file, first_line);
    std::istringstream iss(first_line);
    iss >> row_position >> col_position;
    std::string line;
    while (std::getline(file, line)) {
        vector<bool> row;
        for (char c: line) {
            if (c == '0') {
                row.push_back(false);
            } else if (c == '1') {
                row.push_back(true);
            }
        }
        spacecraft_shape.push_back(row);
    }
    player = new Player(spacecraft_shape, row_position, col_position, player_name);
    file.close();
}

// Function to read celestial objects from a file
void AsteroidDash::read_celestial_objects(const string &input_file) {
    std::ifstream file(input_file);
    std::string line;
    vector<vector<bool> > entire_shape;
    int s = 0;
    int t = 0;
    int object_type = 0;
    while (std::getline(file, line)) {
        if (line.empty()) {
            if (!entire_shape.empty()) {
                CelestialObject *main_object = new CelestialObject(entire_shape, static_cast<ObjectType>(object_type),s, t);
                for(int i=0;i<main_object->shape.size();i++) {
                    for(int j=0; j<main_object->shape[i].size();j++) {
                        if(main_object->shape[i][j]) {
                            main_object->entire_size++;
                        }
                    }
                }
                main_object->current_size=main_object->entire_size;
                if (celestial_objects_list_head == nullptr) {
                    celestial_objects_list_head = main_object;
                } else {
                    CelestialObject *current = celestial_objects_list_head;
                    while (current->next_celestial_object != nullptr) {
                        current = current->next_celestial_object;
                    }
                    current->next_celestial_object = main_object;
                }
                rotate_left(main_object, main_object, s, t);
                entire_shape.clear();
                object_type = 0;
                s = 0;
                t = 0;
            }
        } else {
            vector<bool> row;
            for (char c: line) {
                if (c == '0') {
                    row.push_back(false);
                } else if (c == '1') {
                    row.push_back(true);
                } else if (c == 's') {
                    s = std::stoi(line.substr(2));
                    break;
                } else if (c == 't') {
                    t = std::stoi(line.substr(2));
                    break;
                } else if (c == 'e') {
                    if (line[2] == 'l') {
                        object_type = 1;
                        break;
                    } else {
                        object_type = 2;
                        break;
                    }
                }
            }
            if (!row.empty()) {
                entire_shape.push_back(row);
            }
        }
    }
    if (!entire_shape.empty()) {
        CelestialObject *last_object = new CelestialObject(entire_shape, static_cast<ObjectType>(object_type), s, t);
        for(int i=0;i<last_object->shape.size();i++) {
            for(int j=0; j<last_object->shape[i].size();j++) {
                if(last_object->shape[i][j]) {
                    last_object->entire_size++;
                }
            }
        }
        last_object->current_size=last_object->entire_size;
        if (celestial_objects_list_head == nullptr) {
            celestial_objects_list_head = last_object;
        } else {
            CelestialObject *current = celestial_objects_list_head;
            while (current->next_celestial_object != nullptr) {
                current = current->next_celestial_object;
            }
            current->next_celestial_object = last_object;
        }
        rotate_left(last_object, last_object, s, t);
    }

    file.close();
    CelestialObject *current = celestial_objects_list_head;
    while (current != nullptr) {
        CelestialObject *temp_main = current;
        while (current->right_rotation->shape != temp_main->shape) {
            current->right_rotation->next_celestial_object = current->next_celestial_object;
            current = current->right_rotation;
        }
        current = current->next_celestial_object;
    }
}


void AsteroidDash::rotate_left(CelestialObject *current, CelestialObject *main_object_ref, int s, int t) {
    vector<vector<bool> > left_rotation_shape;
    for (int j = current->shape[0].size() - 1; j >= 0; --j) {
        vector<bool> rotation_row;
        for (int i = 0; i < current->shape.size(); ++i) {
            rotation_row.push_back(current->shape[i][j]);
        }
        left_rotation_shape.push_back(rotation_row);
    }
    if (left_rotation_shape == main_object_ref->shape) {
        current->left_rotation = main_object_ref;
        main_object_ref->right_rotation = current;
    } else {
        CelestialObject *left_object = new CelestialObject(left_rotation_shape, current->object_type, s, t);
        left_object->entire_size=current->entire_size;
        left_object->current_size=current->current_size;
        current->left_rotation = left_object;
        left_object->right_rotation = current;
        left_object->next_celestial_object = main_object_ref->next_celestial_object;
        rotate_left(left_object, main_object_ref, s, t);
    }
}

// Print the entire space grid
void AsteroidDash::print_space_grid() const {
    if(leaderboard.head_leaderboard_entry!=nullptr) {
        cout<<"Tick: "<<game_time<<endl<<"Lives: "<<player->lives<<endl<<"Ammo: "<<player->current_ammo<<endl<<"Score: "<<current_score<<endl<<"High Score: "<<leaderboard.head_leaderboard_entry->score<<endl;
    }
    else {
        cout<<"Tick: "<<game_time<<endl<<"Lives: "<<player->lives<<endl<<"Ammo: "<<player->current_ammo<<endl<<"Score: "<<current_score<<endl<<"High Score: "<<endl;
    }
    for (const auto& row : space_grid) {
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
}


// Function to update the space grid with player, celestial objects, and any other changes
// It is called in every game tick before moving on to the next tick.
void AsteroidDash::update_space_grid() {
    int row_number = space_grid.size();
    int col_number = space_grid[0].size();
    vector<vector<int>> player_cells;
    for (int i = 0; i < row_number; i++) {
        for (int j = 0; j < col_number; j++) {
            space_grid[i][j] = 0;
        }
    }
    CelestialObject *current = celestial_objects_list_head;
    while (current != nullptr) {
        int elapsed_time = static_cast<int>(game_time) - current->time_of_appearance;
        if (elapsed_time>=0 ) {
            if(elapsed_time<current->shape[0].size()+col_number-1) {
                for (int i = 0; i < current->shape.size(); i++) {
                    for (int j = 0; j < current->shape[0].size(); j++) {
                        int new_position=col_number-1 -elapsed_time+j;
                        if (new_position >= 0 && new_position < col_number) {
                            if(space_grid[current->starting_row+i][new_position]==0) {
                                space_grid[current->starting_row+i][new_position] = current->shape[i][j];
                            }
                        }
                    }
                }
            }
        }
        current = current->next_celestial_object;
    }
    for (int i = 0; i < player->spacecraft_shape.size(); i++) {
        for (int j = 0; j < player->spacecraft_shape[i].size(); j++) {
            if (player->spacecraft_shape[i][j] == true) {
                int player_row = player->position_row + i;
                int player_col = player->position_col + j;
                if (space_grid[player_row][player_col] == 1) {
                    CelestialObject *current = celestial_objects_list_head;
                    CelestialObject *previous = nullptr;
                    while (current != nullptr) {
                        int elapsed_time = static_cast<int>(game_time) - current->time_of_appearance;
                        bool delete_object= false;
                        for (int k = 0; k < current->shape.size(); k++) {
                            for (int l = 0; l < current->shape[k].size(); l++) {
                                int obj_row = current->starting_row + k;
                                int obj_col = col_number - 1 - elapsed_time + l;
                                if (obj_row == player_row && obj_col == player_col) {
                                    for (int m = 0; m < current->shape.size(); m++) {
                                        for (int n = 0; n < current->shape[m].size(); n++) {
                                            int clear_row = current->starting_row + m;
                                            int clear_col = col_number - 1 - elapsed_time + n;
                                            if (clear_row >= 0 && clear_row < row_number && clear_col >= 0 && clear_col < col_number) {
                                                space_grid[clear_row][clear_col] = 0;
                                                delete_object=true;
                                            }
                                        }
                                    }
                                    if (current->object_type ==0) {
                                        player->lives --;
                                        if (player->lives==0) {
                                            game_over=true;
                                            return;
                                        }
                                    } else if (current->object_type == 1) {
                                        player->lives ++;
                                    } else if (current->object_type == 2) {
                                        player->current_ammo = player->max_ammo;
                                    }
                                }
                            }
                            if(delete_object) {
                                break;
                            }
                        }
                        if (delete_object) {
                            if (current == celestial_objects_list_head) {
                                celestial_objects_list_head = current->next_celestial_object;
                            } else {
                                previous->next_celestial_object = current->next_celestial_object;
                            }
                            CelestialObject::delete_rotations(current);
                            CelestialObject *temp = current;
                            current = current->next_celestial_object;
                            delete temp;
                        } else {
                            previous = current;
                            current = current->next_celestial_object;
                        }
                    }
                }
            }
        }
    }
    vector<int> shoot_projecetile;
    for(int i=0;i<projectiles.size();i++) {
        bool go_back=false;
        if(projectiles[i][1]+1<col_number) {
            projectiles[i][1]++;
            if((space_grid[projectiles[i][0]][projectiles[i][1]-1]==1&&(i+1>=projectiles.size()||projectiles[i+1][1]!=projectiles[i][1]-1))||(space_grid[projectiles[i][0]][projectiles[i][1]]==1)){
                if(space_grid[projectiles[i][0]][projectiles[i][1]-1]==1) {
                    projectiles[i][1]--;
                    go_back=true;
                }
                CelestialObject *current = celestial_objects_list_head;
                CelestialObject *previous = nullptr;
                bool shoot= false;
                while (current != nullptr) {
                    shoot=false;
                    int elapsed_time = static_cast<int>(game_time) - current->time_of_appearance;
                    for (int k = 0; k < current->shape.size(); k++) {
                        for (int l = 0; l < current->shape[0].size(); l++) {
                            int obj_row = current->starting_row + k;
                            int obj_col = col_number - 1 - elapsed_time + l;
                            if (obj_row == projectiles[i][0] && obj_col==projectiles[i][1]&&space_grid[obj_row][obj_col]==1) {
                                if(current->object_type==0) {
                                    current->shape[k][l] = false;
                                    current_score=current_score+10;
                                    current->current_size--;
                                    if(current->current_size==0) {
                                        current_score=current_score+(100*current->entire_size);
                                    }
                                    shoot=true;
                                    break;
                                }
                            }
                        }
                        if(shoot) {
                            break;
                        }
                    }
                    if (shoot) {
                        shoot_projecetile.push_back(i);
                        double center=current->starting_row+static_cast<double>(current->shape.size()) / 2;
                        space_grid[projectiles[i][0]][projectiles[i][1]] = 0;
                        if(projectiles[i][0]<center) {
                            int odd_center=static_cast<int>(center);
                            if(projectiles[i][0]!=odd_center) {
                                CelestialObject::delete_rotations(current);
                                rotate_left(current,current,current->starting_row,current->time_of_appearance);
                                if(previous!=nullptr) {
                                    previous->next_celestial_object=current->right_rotation;
                                }
                                else {
                                    celestial_objects_list_head=current->right_rotation;
                                }
                                for (int k = 0; k < current->shape.size(); k++) {
                                    for (int l = 0; l < current->shape[k].size(); l++) {
                                        int obj_row = current->starting_row + k;
                                        int obj_col = col_number - 1 - elapsed_time + l;
                                        if (obj_row >= 0 && obj_row < row_number && obj_col >= 0 && obj_col < col_number) {
                                            if (current->shape[k][l]) {
                                                space_grid[obj_row][obj_col] = 0;
                                            }
                                        }
                                    }
                                }
                                for (int k = 0; k < current->right_rotation->shape.size(); k++) {
                                    for (int l = 0; l < current->right_rotation->shape[k].size(); l++) {
                                        int obj_row = current->right_rotation->starting_row + k;
                                        int obj_col = col_number - 1 - elapsed_time + l;
                                        if (obj_row >= 0 && obj_row < row_number && obj_col >= 0 && obj_col < col_number) {
                                            if (current->right_rotation->shape[k][l]) {
                                                space_grid[obj_row][obj_col] = 1;
                                            }
                                        }
                                    }
                                }
                            }else {
                                CelestialObject::delete_rotations(current);
                                rotate_left(current,current,current->starting_row,current->time_of_appearance);
                            }
                        }
                        else {
                            CelestialObject::delete_rotations(current);
                            rotate_left(current,current,current->starting_row,current->time_of_appearance);
                            if(previous!=nullptr) {
                                previous->next_celestial_object=current->left_rotation;
                            }
                            else {
                                celestial_objects_list_head=current->left_rotation;
                            }
                            for (int k = 0; k < current->shape.size(); k++) {
                                for (int l = 0; l < current->shape[k].size(); l++) {
                                    int obj_row = current->starting_row + k;
                                    int obj_col = col_number - 1 - elapsed_time + l;
                                    if (obj_row >= 0 && obj_row < row_number && obj_col >= 0 && obj_col < col_number) {
                                        if (current->shape[k][l]) {
                                            space_grid[obj_row][obj_col] = 0;
                                        }
                                    }
                                }
                            }
                            for (int k = 0; k < current->left_rotation->shape.size(); k++) {
                                for (int l = 0; l < current->left_rotation->shape[k].size(); l++) {
                                    int obj_row = current->left_rotation->starting_row + k;
                                    int obj_col = col_number - 1 - elapsed_time + l;
                                    if (obj_row >= 0 && obj_row < row_number && obj_col >= 0 && obj_col < col_number) {
                                        if (current->left_rotation->shape[k][l]) {
                                            space_grid[obj_row][obj_col] = 1;
                                        }
                                    }
                                }
                            }
                        }
                        break;
                    }
                    previous = current;
                    current = current->next_celestial_object;
                }
                if(!shoot) {
                    if (go_back) {
                        go_back=false;
                        projectiles[i][1]++;
                        space_grid[projectiles[i][0]][projectiles[i][1]] = 1;
                    }
                }
            }
            else {
                space_grid[projectiles[i][0]][projectiles[i][1]] = 1;
            }
        }
        else {
            projectiles.erase(projectiles.begin());
            i--;
        }
    }
    for (int i = shoot_projecetile.size() - 1; i >= 0; i--) {
        projectiles.erase(projectiles.begin() + shoot_projecetile[i]);
    }
    shoot_projecetile.clear();
    for (int i = 0; i < player->spacecraft_shape.size(); i++) {
        for (int j = 0; j < player->spacecraft_shape[i].size(); j++) {
            if (space_grid[player->position_row + i][player->position_col + j] == 0) {
                space_grid[player->position_row + i][player->position_col + j] = player->spacecraft_shape[i][j];
                vector<int> cell;
                cell.push_back(player->position_row+i),
                cell.push_back(player->position_col+j);
                player_cells.push_back(cell);
            }
        }
    }
}

// Corresponds to the SHOOT command.
// It should shoot if the player has enough ammo.
// It should decrease the player's ammo
void AsteroidDash::shoot() {
    if (player->current_ammo>0) {
        player->current_ammo--;
    }
    else {
        return;
    }
    int center=(player->spacecraft_shape.size()/2);
    int projectile_row=player->position_row+center;
    int projectile_col=player->position_col+player->spacecraft_shape[0].size()-1;
    vector<int> projectile;
    projectile.push_back(projectile_row);
    projectile.push_back(projectile_col);
    projectiles.push_back(projectile);
}

// Destructor. Remove dynamically allocated member variables here.
AsteroidDash::~AsteroidDash() {
    delete player;
    CelestialObject *current = celestial_objects_list_head;
    while (current != nullptr) {
        CelestialObject *next = current->next_celestial_object;
        CelestialObject::delete_rotations(current);
        delete current;
        current = next;
    }
}
