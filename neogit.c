#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <ctype.h>

//hint
// line 1 = username
// line 2 = email
//hint
//date , time ,argv[3], id, branch, n_file , user_name
///?????????????????????????????..............free.....................
//void print_command(int argc, char *const argv[]);

int run_init(int argc, char *const argv[]);

int create_configs(char *username, char *email);

int run_config(int argc, char *argv[]);

int creat_alias();

int check_alias(char *command);

const char *true_command(char *command);

int run_add(int argc, char *const argv[]);

int run_reset(int argc, char *const argv[]);

int run_commit(int argc, char *const argv[]);

int run_log(int argc, char *const argv[]);

int run_branch(int argc, char *const argv[]);

int run_checkout(int argc, char *const argv[]);

int run_status(int argc, char *const argv[]);

int run_set(int argc, char *const argv[]);

int run_replace(int argc, char *const argv[]);

int run_remove(int argc, char *const argv[]);

int run_diff(int argc, char *const argv[]);

int run_tag(int argc, char *const argv[]);

void copy_dir(char *address, char *full_address);

int check_time(char *time, char *committed_time);

int isWhitespaceLine(const char *line);
/////////////////////////////////////////////////////////////////////////////////////

//void print_command(int argc, char *const argv[]) {
//    for (int i = 0; i < argc; i++) {
//        fprintf(stdout, "%s ", argv[i]);
//    }
//    fprintf(stdout, "\n");
//}

/////////////////////////////////////////////////////////////////////////////////

const char *true_command(char *command) {
    FILE *file = fopen(".neogit/alias", "r");
    //if (file == NULL) return 1;
    char *alias_content;
    alias_content = (char *) malloc(10000 * sizeof(char));
    int flag = 0;
    char *true_command2;
    true_command2 = (char *) malloc(10000 * sizeof(char));
    char *proj_commends = (char *) malloc(10000 * sizeof(char));
    while (fgets(alias_content, 10000, file) != NULL) {
        if (alias_content[strlen(alias_content) - 1] == '\n')
            alias_content[strlen(alias_content) - 1] = '\0';
        char *token = strtok(alias_content, " ");
        strcpy(proj_commends, token);
        while (token != NULL) {
            if (strcmp(command, token) == 0) {
                flag = 1;
                strcpy(true_command2, proj_commends);
            }
            token = strtok(NULL, " ");
        }
    }
    fclose(file);
    if (flag == 0) { printf("invalid command\n"); }
    else
        return true_command2;
}

/////////////////////////////////////////////////////////////////////////////////

int check_alias(char *command) {
    FILE *file = fopen("/home/aryana/Desktop/project/data/commands", "r");
    if (file == NULL) return 1;
    char *commands_content;
    commands_content = (char *) malloc(10000 * sizeof(char));
    int flag = 0;
    int counter = 0;
    while (fgets(commands_content, 10000, file) != NULL) {
        if (commands_content[strlen(commands_content) - 1] == '\n')
            commands_content[strlen(commands_content) - 1] = '\0';
        counter++;
        if (strstr(command, commands_content) != NULL) {
            flag = 1;
            break;
        }
    }
    fclose(file);
    if (flag == 0)
        return flag;
    else
        return counter;
}

/////////////////////////////////////////////////////////////////////////////////

int check_time(char *time, char *committed_time) {
    char *token = strtok(time, "-");
    int year = atoi(token);
    token = strtok(NULL, "-");
    int month = atoi(token);
    token = strtok(NULL, "-");
    int day = atoi(token);
    char *tok = strtok(committed_time, "-");
    int year2 = atoi(tok);
    tok = strtok(NULL, "-");
    int month2 = atoi(tok);
    tok = strtok(NULL, "-");
    int day2 = atoi(tok);
//    printf("%d %d %d \n" , year , month , day);
//    printf("%d %d %d\n" , year2 , month2 , day2);
    if (year2 > year)
        return 1;
    if (year2 < year)
        return 0;
    if (month2 > month)
        return 1;
    if (month2 < month)
        return 0;
    if (day2 > day)
        return 1;
    if (day2 < day)
        return 0;
    return -1;
}

/////////////////////////////////////////////////////////////////////////////////

int isWhitespaceLine(const char *line) {
    while (*line != '\0') {
        if (!isspace((unsigned char) *line)) {
            return 0;
        }
        line++;
    }
    return 1;
}

/////////////////////////////////////////////////////////////////////////////////

int run_init(int argc, char *const argv[]) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) return 1;
    FILE *add = fopen("/home/aryana/Desktop/project/data/address", "a");
    fprintf(add, "%s\n", cwd);
    char tmp_cwd[1024];
    bool exists = false;
    struct dirent *entry;
    do {
        //find .neogit
        DIR *dir = opendir(".");
        if (dir == NULL) {
            perror("Error opening current directory");
            return 1;
        }
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".neogit") == 0)
                exists = true;
        }
        closedir(dir);

        ///update current working directory
        if (getcwd(tmp_cwd, sizeof(tmp_cwd)) == NULL) return 1;

        // change cwd to parent
        if (strcmp(tmp_cwd, "/") != 0) {
            if (chdir("..") != 0) return 1;
        }

    } while (strcmp(tmp_cwd, "/") != 0);

    // return to the initial cwd
    if (chdir(cwd) != 0) return 1;
    if (!exists) {
        if (mkdir(".neogit", 0755) != 0) return 1;
        creat_alias();
        FILE *file = fopen("/home/aryana/Desktop/project/data/config", "r");
        char *name = (char *) malloc(10000 * sizeof(char));
        fgets(name, 10000, file);
        if (name[strlen(name) - 1] == '\n')
            name[strlen(name) - 1] = '\0';
        char *email = (char *) malloc(10000 * sizeof(char));
        fgets(email, 10000, file);
        if (email[strlen(email) - 1] == '\n')
            email[strlen(email) - 1] = '\0';
        return create_configs(name, email);
        fclose(file);
    } else {
        perror("neogit repository has already initialized");
    }
    fclose(add);
    return 0;
}

//////////////////////////////////////////////////////////////////////////////

int create_configs(char *username, char *email) {
    FILE *file = fopen(".neogit/config", "w");
    if (file == NULL) return 1;

    fprintf(file, "%s\n", username);
    fprintf(file, "%s\n", email);
    fprintf(file, "last_commit_ID: %d\n", 0);
    fprintf(file, "current_commit_ID: %d\n", 0);
    fprintf(file, "branch: %s", "master");

    fclose(file);

    // create commits folder
    if (mkdir(".neogit/branches", 0755) != 0) return 1;
    if (mkdir(".neogit/branches/master", 0755) != 0) return 1;
    file = fopen(".neogit/branches/master/this_branch_id", "w");
    fprintf(file, "%s\n", "0");
    fclose(file);
    if (mkdir(".neogit/branches/master/commits", 0755) != 0) return 1;

    file = fopen(".neogit/current_commit_id", "w");
    fprintf(file, "%s\n", "0");
    fclose(file);

    file = fopen(".neogit/commits_id", "w");
    fprintf(file, "%s\n", "1");
    fclose(file);
    file = fopen(".neogit/commit_shortcut", "w");
    fclose(file);
    file = fopen(".neogit/commits_addressandid", "w");
    fprintf(file, "%s\n", "0");
    fclose(file);
    file = fopen(".neogit/all_branches", "w");
    fprintf(file, "%s\n", "master");
    fclose(file);
    file = fopen(".neogit/branch_name", "w");
    fprintf(file, "%s\n", "master");
    fclose(file);

    //for tag
    if (mkdir(".neogit/tags", 0755) != 0) return 1;
    file = fopen(".neogit/tags/tag_name", "w");
    fprintf(file, "%s\n", "0");
    fclose(file);
    file = fopen(".neogit/tags/tag_data", "w");
    fprintf(file, "%s\n", "0");
    fclose(file);

    // create files folder
    //if (mkdir(".neogit/files", 0755) != 0) return 1;

    if (mkdir(".neogit/staging", 0755) != 0) return 1;

    if (mkdir(".neogit/unstaging", 0755) != 0) return 1;
    file = fopen(".neogit/unstag_address", "w");  //////////////////////////////////////////dicteh
    fclose(file);

    //file = fopen(".neogit/tracks", "w");
    //fclose(file);

    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////

int creat_alias() {
    FILE *file = fopen(".neogit/alias", "w");
    if (file == NULL) return 1;
    fprintf(file, "init\n");
    fprintf(file, "config\n");
    fprintf(file, "add\n");
    fprintf(file, "reset\n");
    fprintf(file, "status\n");
    fprintf(file, "commit\n");
    fprintf(file, "log\n");
    fprintf(file, "branch\n");
    fprintf(file, "checkout\n");
    fprintf(file, "set\n");
    fprintf(file, "replace\n");
    fprintf(file, "remove\n");
    fprintf(file, "diff\n");
    //fprintf(file, "set\n");
    fclose(file);
}

/////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////

int run_config(int argc, char *argv[]) {
    if (argc < 4)
        printf("invalid command\n");
    else if (argc == 4) {
        if (strcmp(argv[2], "user.name") == 0) {
            FILE *file;
            file = fopen(".neogit/config", "r");
            if (file == NULL) return 1;
            FILE *file2;
            file2 = fopen(".neogit/config2", "w");
            char *file_content = (char *) malloc(10000 * sizeof(char));

            fprintf(file2, "%s\n", argv[3]);
            fgets(file_content, 10000, file);
            for (int i = 0; i < 4; i++) {
                fgets(file_content, 10000, file);
                if (file_content[strlen(file_content) - 1] == '\n')
                    file_content[strlen(file_content) - 1] = '\0';
                fprintf(file2, "%s\n", file_content);
            }
            fclose(file);
            remove(".neogit/config");
            fclose(file2);
            rename(".neogit/config2", ".neogit/config");
        } else if (strcmp(argv[2], "user.email") == 0) {
            FILE *file;
            file = fopen(".neogit/config", "r");
            if (file == NULL) return 1;
            FILE *file2;
            file2 = fopen(".neogit/config2", "w");
            char *file_content = (char *) malloc(10000 * sizeof(char));
            fgets(file_content, 10000, file);
            if (file_content[strlen(file_content) - 1] == '\n')
                file_content[strlen(file_content) - 1] = '\0';
            fprintf(file2, "%s\n", file_content);
            fprintf(file2, "%s\n", argv[3]);
            fgets(file_content, 10000, file);
            for (int i = 0; i < 3; i++) {
                fgets(file_content, 10000, file);
                if (file_content[strlen(file_content) - 1] == '\n')
                    file_content[strlen(file_content) - 1] = '\0';
                fprintf(file2, "%s\n", file_content);
            }
            fclose(file);
            remove(".neogit/config");     ////////?
            fclose(file2);
            rename(".neogit/config2", ".neogit/config");
        } else {
            printf("invalid command\n");
        }
    } else if (argc == 5) {
        if (strcmp(argv[2], "-global") == 0) {
            if (strcmp(argv[3], "user.name") == 0) {
                char *neogit_addresses;
                neogit_addresses = (char *) malloc(10000 * sizeof(char));
                FILE *find = fopen("/home/aryana/Desktop/project/data/address", "r");
                if (find == NULL) return 1;
                while (fgets(neogit_addresses, 10000, find) != NULL) {
                    //printf("1\n");
                    if (neogit_addresses[strlen(neogit_addresses) - 1] == '\n')
                        neogit_addresses[strlen(neogit_addresses) - 1] = '\0';
                    char *neogit_addresses2;
                    neogit_addresses2 = (char *) malloc(10000 * sizeof(char));
                    strcpy(neogit_addresses2, neogit_addresses);
                    strcat(neogit_addresses2, "/.neogit/config");
                    FILE *file = fopen(neogit_addresses2, "r");
                    if (file == NULL) return 1;
                    strcat(neogit_addresses, "/.neogit/config2");
                    FILE *file2 = fopen(neogit_addresses, "w");   /////////free neogit addrese
                    fprintf(file2, "%s\n", argv[4]);
                    char *file_content = (char *) malloc(10000 * sizeof(char));
                    fgets(file_content, 10000, file);
                    for (int i = 0; i < 4; i++) {
                        fgets(file_content, 10000, file);
                        if (file_content[strlen(file_content) - 1] == '\n')
                            file_content[strlen(file_content) - 1] = '\0';
                        fprintf(file2, "%s\n", file_content);
                    }
                    fclose(file);
                    remove(neogit_addresses2);
                    fclose(file2);
                    rename(neogit_addresses, neogit_addresses2);
                    //printf("1\n");
                }
                fclose(find);
                FILE *file = fopen("/home/aryana/Desktop/project/data/config", "r");
                if (file == NULL) return 1;
                FILE *file2 = fopen("/home/aryana/Desktop/project/data/config2", "w");
                fprintf(file2, "%s\n", argv[4]);
                char *proj_config = (char *) malloc(10000 * sizeof(char));
                fgets(proj_config, 10000, file);
                if (proj_config[strlen(proj_config) - 1] == '\n')
                    proj_config[strlen(proj_config) - 1] = '\0';
                fgets(proj_config, 10000, file);
                if (proj_config[strlen(proj_config) - 1] == '\n')
                    proj_config[strlen(proj_config) - 1] = '\0';
                fprintf(file2, "%s\n", proj_config);
                fclose(file);
                remove("/home/aryana/Desktop/project/data/config");
                fclose(file2);
                rename("/home/aryana/Desktop/project/data/config2", "/home/aryana/Desktop/project/data/config");
            } else if (strcmp(argv[3], "user.email") == 0) {
                char *neogit_addresses;
                neogit_addresses = (char *) malloc(10000 * sizeof(char));
                FILE *find = fopen("/home/aryana/Desktop/project/data/address", "r");
                if (find == NULL) return 1;
                while (1) {
                    fgets(neogit_addresses, 10000, find);
                    if (neogit_addresses == NULL)
                        break;
                    if (neogit_addresses[strlen(neogit_addresses) - 1] == '\n')
                        neogit_addresses[strlen(neogit_addresses) - 1] = '\0';
                    char *neogit_addresses2;
                    neogit_addresses2 = (char *) malloc(10000 * sizeof(char));
                    strcpy(neogit_addresses2, neogit_addresses);
                    strcat(neogit_addresses2, "/.neogit/config");
                    FILE *file = fopen(neogit_addresses2, "r");
                    if (file == NULL) return 1;
                    strcat(neogit_addresses, "/.neogit/config2");
                    FILE *file2 = fopen(neogit_addresses, "w");
                    char *file_content = (char *) malloc(10000 * sizeof(char));
                    fgets(file_content, 10000, file);
                    if (file_content[strlen(file_content) - 1] == '\n')
                        file_content[strlen(file_content) - 1] = '\0';
                    fprintf(file2, "%s\n", file_content);
                    fprintf(file2, "%s\n", argv[4]);
                    fgets(file_content, 10000, file);
                    for (int i = 0; i < 3; i++) {
                        fgets(file_content, 10000, file);
                        if (file_content[strlen(file_content) - 1] == '\n')
                            file_content[strlen(file_content) - 1] = '\0';
                        fprintf(file2, "%s\n", file_content);
                    }
                    fclose(file);
                    remove(neogit_addresses2);
                    fclose(file2);
                    rename(neogit_addresses, neogit_addresses2);
                }
                FILE *file = fopen("/home/aryana/Desktop/project/data/config", "r");
                if (file == NULL) return 1;
                FILE *file2 = fopen("/home/aryana/Desktop/project/data/config2", "w");
                if (file2 == NULL) return 1;
                char *proj_config = (char *) malloc(10000 * sizeof(char));
                fgets(proj_config, 10000, file);
                if (proj_config[strlen(proj_config) - 1] == '\n')
                    proj_config[strlen(proj_config) - 1] = '\0';
                fprintf(file2, "%s\n", proj_config);
                fprintf(file2, "%s\n", argv[4]);
                fclose(file);
                remove("/home/aryana/Desktop/project/data/config");
                fclose(file2);
                rename("/home/aryana/Desktop/project/data/config2", "/home/aryana/Desktop/project/data/config");
                return 0;
            } else {
                printf("invalidd command\n");
            }

        } else if (strcmp(argv[2], "alias") == 0) {
            int result = check_alias(argv[4]);
            if (result == 0)
                printf("invalid command\n");
            else {
                FILE *file = fopen(".neogit/alias", "r");
                if (file == NULL) return 1;
                FILE *file2 = fopen(".neogit/alias2", "w");
                char *file_content = (char *) malloc(10000 * sizeof(char));
                printf("check_alias(argv[4]) = %d\n", result);
                for (int i = 0; i < result - 1; i++) {
                    fgets(file_content, 10000, file);
                    if (file_content[strlen(file_content) - 1] == '\n')
                        file_content[strlen(file_content) - 1] = '\0';
                    fprintf(file2, "%s\n", file_content);
                }
                fgets(file_content, 10000, file);
                if (file_content[strlen(file_content) - 1] == '\n')
                    file_content[strlen(file_content) - 1] = '\0';
                fprintf(file2, "%s ", file_content);
                fprintf(file2, "%s\n", argv[3]);
                for (int i = result; i < 9; i++) {
                    fgets(file_content, 10000, file);
                    if (file_content[strlen(file_content) - 1] == '\n')
                        file_content[strlen(file_content) - 1] = '\0';
                    fprintf(file2, "%s\n", file_content);
                }
                fclose(file);
                remove(".neogit/alias");
                fclose(file2);
                rename(".neogit/alias2", ".neogit/alias");
            }
        } else {
            printf("invalid coommand\n");
        }
    } else if (argc == 6) {
        if ((strcmp(argv[3], "alias") == 0) && (strcmp(argv[2], "-global") == 0)) {
            int result = check_alias(argv[5]);
            if (result == 0)
                printf("invalid command\n");
            else {     ///////////////////////////////////////////////////////
//                FILE *file_proj= fopen("/home/aryana/Desktop/project/data/alias", "r");
//                FILE *file_proj2= fopen("/home/aryana/Desktop/project/data/alias2", "w");
//                if (file_proj == NULL) return 1;
//                char *file_content0 = (char *) malloc(10000 * sizeof(char));
//                for (int i = 0; i < result - 1; i++) {
//                    fgets(file_content0, 10000, file_proj);
//                    if (file_content0[strlen(file_content0) - 1] == '\n')
//                        file_content0[strlen(file_content0) - 1] = '\0';
//                    fprintf(file_proj2, "%s\n", file_content0);
//                }
//                fgets(file_content0, 10000, file_proj);
//                if (file_content0[strlen(file_content0) - 1] == '\n')
//                    file_content0[strlen(file_content0) - 1] = '\0';
//                fprintf(file_proj2, "%s ", file_content0);
//                fprintf(file_proj2, "%s\n", argv[4]);
//                for (int i = result; i < 9; i++) {
//                    fgets(file_content0, 10000, file_proj);
//                    if (file_content0[strlen(file_content0) - 1] == '\n')
//                        file_content0[strlen(file_content0) - 1] = '\0';
//                    fprintf(file_proj2, "%s\n", file_content0);
//                }
//                fclose(file_proj);
//                remove("/home/aryana/Desktop/project/data/alias");
//                fclose(file_proj2);
//                rename("/home/aryana/Desktop/project/data/alias2", "/home/aryana/Desktop/project/data/alias");
                //////////////////////////////////////////////////////////////////////////
                printf("1\n");
                char *neogit_addresses;
                neogit_addresses = (char *) malloc(10000 * sizeof(char));
                FILE *find = fopen("/home/aryana/Desktop/project/data/address", "r");
                if (find == NULL) return 1;
                while (1) {
                    fgets(neogit_addresses, 10000, find);
                    if (neogit_addresses == NULL)
                        break;
                    if (neogit_addresses[strlen(neogit_addresses) - 1] == '\n')
                        neogit_addresses[strlen(neogit_addresses) - 1] = '\0';
                    char *neogit_addresses2;
                    neogit_addresses2 = (char *) malloc(10000 * sizeof(char));
                    strcpy(neogit_addresses2, neogit_addresses);
                    printf("%s , %s\n", neogit_addresses, neogit_addresses2);
                    strcat(neogit_addresses2, "/.neogit/alias");
                    printf("%s , %s\n", neogit_addresses, neogit_addresses2);
                    FILE *file = fopen(neogit_addresses2, "r");
                    if (file == NULL) return 1;///////////////////////////////////////////?????
                    strcat(neogit_addresses, "/.neogit/alias2");
                    printf("%s , %s\n", neogit_addresses, neogit_addresses2);
                    FILE *file2 = fopen(neogit_addresses, "w");
                    char *file_content = (char *) malloc(10000 * sizeof(char));
                    for (int i = 0; i < result - 1; i++) {
                        fgets(file_content, 10000, file);
                        if (file_content[strlen(file_content) - 1] == '\n')
                            file_content[strlen(file_content) - 1] = '\0';
                        fprintf(file2, "%s\n", file_content);
                    }
                    fgets(file_content, 10000, file);
                    if (file_content[strlen(file_content) - 1] == '\n')
                        file_content[strlen(file_content) - 1] = '\0';
                    fprintf(file2, "%s ", file_content);
                    fprintf(file2, "%s\n", argv[4]);
                    for (int i = result; i < 9; i++) {
                        fgets(file_content, 10000, file);
                        if (file_content[strlen(file_content) - 1] == '\n')
                            file_content[strlen(file_content) - 1] = '\0';
                        fprintf(file2, "%s\n", file_content);
                    }
                    fclose(file);
                    remove(neogit_addresses2);
                    fclose(file2);
                    rename(neogit_addresses, neogit_addresses2);
                    /////////////////////////////////////////////////////////////
                }
            }

        } else {
            printf("invalid command\n");
        }
    } else {
        printf("invalid command\n");
    }
    return 0;
}/////////bag dare???

/////////////////////////////////////////////////////////////////////////////////////

int run_add(int argc, char *const argv[]) {
    if (argc < 3)
        printf("invalid command\n");
    int i = 2;
    if ((strcmp(argv[2], "-n") == 0)) {
        int flag = 0;
        if (argc < 4 || atoi(argv[3]) != 1) {
            printf("invalid command\n");
            return 1;
        }
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) == NULL) return 1;
        //printf("cwd = %s\n", cwd);
        FILE *check = fopen(".neogit/add_address", "r");
        bool exists = false;
        struct dirent *entry;
        //find .neogit
        DIR *dir = opendir(cwd);
        if (dir == NULL) {
            perror("Error opening current directory");
            return 1;
        }
        char *addaddress_content = (char *) malloc(10000 * (sizeof(char)));
        while ((entry = readdir(dir)) != NULL) {
            flag = 0;
            if (entry->d_name[0] != '.') {
                printf("%s  ", entry->d_name);
                char *fd_address = (char *) malloc(10000 * (sizeof(char)));
                strcpy(fd_address, cwd);
                strcat(fd_address, "/");
                strcat(fd_address, entry->d_name);
                while (fgets(addaddress_content, 10000, check) != NULL) {
                    if (addaddress_content[strlen(addaddress_content) - 1] == '\n')
                        addaddress_content[strlen(addaddress_content) - 1] = '\0';
                    //printf("%s \n  %s\n", addaddress_content, fd_address);
                    if (strcmp(addaddress_content, fd_address) == 0) {
                        printf("staged\n");
                        flag = 2;
                        break;
                    } else if (strstr(addaddress_content, fd_address) !=
                               NULL) { //addaddress_content[strlen(fd_address)] ==
                        flag = 1;
                        break;
                    }
                }
                if (flag == 1) { printf("there is a file or directory in this directory which is staged\n"); }
                if (flag == 0) {
                    printf("not staged\n");

                }
                rewind(check);
            }
        }
        fclose(check);
        closedir(dir);

        return 0;
    } else if ((strcmp(argv[2], "-redo") == 0)) {

        struct dirent *entry;
        DIR *dir = opendir(".neogit/unstaging");
        if (dir == NULL) {
            perror("Error opening current directory");
            return 1;
        }
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_name[0] != '.') {
                char *address_of_the_unstaged_file = (char *) malloc(10000 * sizeof(char));
                strcpy(address_of_the_unstaged_file, ".neogit/unstaging/");
                strcat(address_of_the_unstaged_file, entry->d_name);
                char command[10000] = "";
                sprintf(command, "rsync -r %s %s", address_of_the_unstaged_file, ".neogit/staging");
                system(command);
            }
        }
        closedir(dir);
        return 0;
    } else if ((strcmp(argv[2], "-f") == 0)) {
        if (argc < 4) {
            printf("invalid command\n");
            return 1;
        }
        i = 3;
    }
    for (i; i < argc; i++) {     ////////////////////////////////////////3
        int flag = 0;
        int flag_slash = 1;
        char *address = (char *) malloc(10000 * sizeof(char));
        strcpy(address, argv[i]);
        char *name = (char *) malloc(10000 * sizeof(char));
        if (strstr(address, "/") != NULL)
            name = strrchr(address, '/');
            //if (name == NULL)
        else {
            strcpy(name, address);
            flag_slash = 0;
        }
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) == NULL) return 1;
        char tmp_cwd[1024];
        strcpy(tmp_cwd, cwd);
        strcat(tmp_cwd, "/");
        strcat(tmp_cwd, address);
        //bool exists = false;
        struct dirent *entry;

        //find the file or directory
        DIR *dir = opendir(tmp_cwd);
        if (dir == NULL) {
            closedir(dir);
            FILE *file = fopen(tmp_cwd, "r");
            flag = 1;
            if (file == NULL) {
                if ((strcmp(argv[2], "-f") == 0))
                    printf("file or directory num %d doesn't exist\n", i - 2);
                else
                    printf("file or directory num %d doesn't exist\n", i - 1);
                continue;
            }
            fclose(file);
        }
        //printf("flag = %d\n" , flag);
        FILE *file = fopen(".neogit/add_address", "a");
        if (file == NULL) return 1;
        fprintf(file, "%s\n", tmp_cwd);
        fclose(file);////////////////////////////////////////////
        if (flag == 0) {

            char *stage_address = (char *) malloc(10000 * sizeof(char));
            strcpy(stage_address, ".neogit/staging");
            //stage_address = ".neogit/staging";
            strcat(stage_address, "/");
            strcat(stage_address, address);
            char command[10000] = "";
            sprintf(command, "rsync -r %s %s", address, ".neogit/staging");
            system(command);

        }
        if (flag == 1) {
            //printf("its a file\n");
            file = fopen(address, "r");
            if (file == NULL) return 1; ////////////????????????????????????
            char *copy_address = (char *) malloc(10000 * sizeof(char));
            strcpy(copy_address, ".neogit/staging");
            //printf("%s\n" , copy_address);
            if (flag_slash == 1)
                strcat(copy_address, name);
            else {
                strcat(copy_address, "/");
                strcat(copy_address, name);
            }
            //printf("%s\n" , copy_address);
            FILE *stage = fopen(copy_address, "w");
            char *file_content = (char *) malloc(10000 * sizeof(char));
            while (fgets(file_content, 10000, file) != NULL) {
                if (file_content[strlen(file_content) - 1] == '\n')
                    file_content[strlen(file_content) - 1] = '\0';
                fprintf(stage, "%s\n", file_content);
            }
            fclose(file);
            fclose(stage);
        }
    }
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////

int run_reset(int argc, char *const argv[]) {
    if (argc != 3)
        printf("invalid command\n");
    else {
        if (strcmp(argv[2], "-undo") != 0) {
            int flag = 0;
            int flag_slash = 1;
            char *address = (char *) malloc(10000 * sizeof(char));
            strcpy(address, argv[2]);
            char *name = (char *) malloc(10000 * sizeof(char));
            if (strstr(address, "/") != NULL)
                name = strrchr(address, '/');
                //if (name == NULL)
            else {
                strcpy(name, address);
                flag_slash = 0;
            }
            char cwd[1024];
            if (getcwd(cwd, sizeof(cwd)) == NULL) return 1;
            char tmp_cwd[1024];
            strcpy(tmp_cwd, cwd);
            strcat(tmp_cwd, "/");
            strcat(tmp_cwd, address);
            //bool exists = false;
            struct dirent *entry;

            //find the file or directory
            DIR *dir = opendir(tmp_cwd);
            if (dir == NULL) {
                closedir(dir);
                FILE *file = fopen(tmp_cwd, "r");
                flag = 1;
                if (file == NULL) {
                    printf("file or directory doesn't exist\n");
                    return 1;
                }
                fclose(file);
            }
            if (flag == 1) {
                FILE *address2 = fopen(".neogit/add_address", "r");  ///fclose
                if (address2 == NULL) return 1;
                char *addaddress_content = (char *) malloc(10000 * (sizeof(char)));
                while (fgets(addaddress_content, 10000, address2) != NULL) {
                    if (addaddress_content[strlen(addaddress_content) - 1] == '\n')
                        addaddress_content[strlen(addaddress_content) - 1] = '\0';

                    if (strcmp(addaddress_content, tmp_cwd) == 0) {
                        //FILE * staged = fopen(".neogit/staging" ,);
                        char *stage_address = (char *) malloc(10000 * (sizeof(char)));
                        strcpy(stage_address, ".neogit/staging");
                        char *unstage_address = (char *) malloc(10000 * (sizeof(char)));
                        strcpy(unstage_address, ".neogit/unstaging");
                        if (flag_slash == 0) {
                            strcat(stage_address, "/");
                            strcat(stage_address, name);
                        } else
                            strcat(stage_address, name);
                        if (flag_slash == 0) {
                            strcat(unstage_address, "/");
                            strcat(unstage_address, name);
                        } else
                            strcat(unstage_address, name);
                        FILE *unstage = fopen(unstage_address, "w");   ////////fclose
                        char *file_content = (char *) malloc(10000 * (sizeof(char)));
                        FILE *file = fopen(tmp_cwd, "r");
                        while (fgets(file_content, 10000, file) != NULL) {
                            if (file_content[strlen(file_content) - 1] == '\n')
                                file_content[strlen(file_content) - 1] = '\0';
                            fprintf(unstage, "%s\n", file_content);
                        }
                        FILE *last_unstaged = fopen(".neogit/unstag_address", "w");
                        fprintf(last_unstaged, "%s\n", unstage_address);
                        fclose(last_unstaged);
                        fclose(unstage);
                        fclose(file);
                        fclose(address2);
                        remove(stage_address);
                        return 0;
                    }
                }
                fclose(address2);
                printf("the file is already unstaged\n");
                return 0; ////////////////??????????????????/check?
            } else if (flag == 0) {
                FILE *address2 = fopen(".neogit/add_address", "r");
                if (address2 == NULL) return 1;
                char *addaddress_content = (char *) malloc(10000 * (sizeof(char)));
                while (fgets(addaddress_content, 10000, address2) != NULL) {
                    if (addaddress_content[strlen(addaddress_content) - 1] == '\n')
                        addaddress_content[strlen(addaddress_content) - 1] = '\0';
                    if (strcmp(addaddress_content, tmp_cwd) == 0) {
                        char *stage_address = (char *) malloc(10000 * sizeof(char));
                        char *unstage_address = (char *) malloc(10000 * sizeof(char));
                        strcpy(unstage_address, ".neogit/unstaging");
                        strcpy(stage_address, ".neogit/staging");///////////?/??????????????????
                        //stage_address = ".neogit/staging";
                        strcat(stage_address, "/");
                        strcat(stage_address, address);
                        char command[10000] = "";
                        sprintf(command, "rsync -r %s %s", address, ".neogit/unstaging");
                        system(command);
                        char command2[10000] = "";
                        if (flag_slash == 0) {
                            strcat(stage_address, "/");
                            strcat(stage_address, name);
                        } else
                            strcat(stage_address, name);
                        if (flag_slash == 0) {
                            strcat(unstage_address, "/");
                            strcat(unstage_address, name);
                        } else
                            strcat(unstage_address, name);
                        if (flag_slash == 0)
                            sprintf(command2, "rm -r .neogit/staging/%s/", name);
                        else
                            sprintf(command2, "rm -r .neogit/staging/%s", name);
                        system(command2);
                        FILE *last_unstaged = fopen(".neogit/unstag_address", "w");
                        fprintf(last_unstaged, "%s\n", unstage_address);
                        fclose(last_unstaged);
                        return 0;
                    }
                }
                fclose(address2);
                printf("the directory is already unstaged\n");
                return 0;
            }
        } else if (strcmp(argv[2], "-undo") == 0) {

            FILE *last_unstaged = fopen(".neogit/unstag_address", "r"); //////close
            if (last_unstaged == NULL)
                return 1;////////////////////////////??????????????????????
            char *last_unstaged_address = (char *) malloc(10000 * sizeof(char));
            if (fgets(last_unstaged_address, 10000, last_unstaged) == NULL)
                return 1;
            if (last_unstaged_address[strlen(last_unstaged_address) - 1] == '\n')
                last_unstaged_address[strlen(last_unstaged_address) - 1] = '\0';
            fclose(last_unstaged);
            int flag = 0;
            DIR *dir = opendir(last_unstaged_address);
            if (dir == NULL) {
                closedir(dir);
                FILE *file = fopen(last_unstaged_address, "r");
                flag = 1;
                if (file == NULL) {
                    printf("file or directory doesn't exist\n");
                    return 1;
                }

                fclose(file);
            }
            printf("%d\n", flag);
            if (flag == 1) {
                char command[10000] = "";
                sprintf(command, "rsync -r %s %s", last_unstaged_address, ".neogit/staging");
                system(command);
                char command2[10000] = "";
                sprintf(command2, "rm -r %s", last_unstaged_address);
                system(command2);
                return 0;
            } else if (flag == 0) {
                char command[10000] = "";
                sprintf(command, "rsync -r %s %s", last_unstaged_address, ".neogit/staging");
                system(command);
                char command2[10000] = "";
                sprintf(command2, "rm -r %s", last_unstaged_address);
                system(command2);
                return 0;
            }
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////

int run_commit(int argc, char *const argv[]) {

    if (argc < 4) {
        printf("invalid command\n");
        return 1;
    } else if (argc > 4) {
        printf("please put your commit message between \"\"\n");
        return 1;
    }
    if ((strcmp(argv[2], "-m") != 0) && (strcmp(argv[2], "-s") != 0)) {
        printf("invalid command\n");
        return 1;
    }
    if ((strcmp(argv[2], "-m") == 0)) {
        if (strlen(argv[3]) > 72) {
            printf("your commit message has too many characters\n");
            return 1;
        }
    }
    char *message = (char *) malloc(10000 * sizeof(char));
    if ((strcmp(argv[2], "-s") == 0)) {

        //open shortcut file
        FILE *file = fopen(".neogit/commit_shortcut", "r");

        //find shortcut
        int flag = 0;
        char *shortcut = (char *) malloc(10000 * sizeof(char));
        while (fgets(shortcut, 10000, file) != NULL) {
            if (shortcut[strlen(shortcut) - 1] == '\n')
                shortcut[strlen(shortcut) - 1] = '\0';
            if (strcmp(shortcut, argv[3]) == 0) {
                fgets(shortcut, 10000, file);
                if (shortcut[strlen(shortcut) - 1] == '\n')
                    shortcut[strlen(shortcut) - 1] = '\0';
                flag = 1;
                break;
            }
        }
        fclose(file);

        if (flag == 0) {
            printf("the shortcut doesnt exist\n");
            return 1;
        }
        strcpy(message, shortcut);
    } else if ((strcmp(argv[2], "-m") == 0)) {
        strcpy(message, argv[3]);
    }
    //
    char *commit_address = (char *) malloc(10000 * sizeof(char));
    strcpy(commit_address, ".neogit/branches/");

    char *which_branch = (char *) malloc(10000 * sizeof(char));
    FILE *file = fopen(".neogit/branch_name", "r");
    fgets(which_branch, 10000, file);
    if (which_branch[strlen(which_branch) - 1] == '\n')
        which_branch[strlen(which_branch) - 1] = '\0';

    strcat(commit_address, which_branch);
    char *branch_id_address = (char *) malloc(10000 * sizeof(char));
    strcpy(branch_id_address, commit_address);
    //printf("%s\n" , commit_address); ta khode branch e jadid
    strcat(commit_address, "/commits/");
    fclose(file);
    file = fopen(".neogit/commits_id", "r");
    char *id2 = (char *) malloc(1000 * sizeof(char));
    fgets(id2, 1000, file);
    if (id2[strlen(id2) - 1] == '\n')
        id2[strlen(id2) - 1] = '\0';
    int id;
    id = atoi(id2);
    //printf("%d\n", id);
    fclose(file);
    //printf("1\n");
    char *str_id = (char *) malloc(10 * sizeof(char));
    sprintf(str_id, "%d", id);
    strcat(commit_address, str_id);////////////address id

    bool exists = false;
    struct dirent *entry;
    //printf("1\n");
    DIR *dir = opendir(".neogit/staging");//////????????????
    if (dir == NULL) {
        perror("Error opening staging directory");
        return 1;
    }
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] != '.')////////////stage hidden?
            exists = true;
    }
    closedir(dir);
    if (!exists) {
        perror("no file or directory has been staged yet\n");
        return 1;
    } else {


        char *file_commit_content = (char *) malloc(100 * sizeof(char));
        file = fopen(".neogit/commits_addressandid", "r");
        if (file == NULL)
            return 1;

        FILE *file2 = fopen(".neogit/commits_addressandid2", "w");
        fprintf(file2, "%s ", str_id);
        fprintf(file2, "%s\n", commit_address);

        while (fgets(file_commit_content, 100, file) != NULL) {
            if (file_commit_content[strlen(file_commit_content) - 1] == '\n')
                file_commit_content[strlen(file_commit_content) - 1] = '\0';
            fprintf(file2, "%s\n", file_commit_content);
        }
        fclose(file);
        remove(".neogit/commits_addressandid");
        fclose(file2);
        rename(".neogit/commits_addressandid2", ".neogit/commits_addressandid");

        strcat(branch_id_address, "/this_branch_id");

        if (mkdir(commit_address, 0755) != 0) return 1;
        //copy last commit
        if (id > 1) {
            char *old_commit_address = (char *) malloc(10000 * sizeof(char));

            strcpy(old_commit_address, ".neogit/branches/");
            strcat(old_commit_address, which_branch);
            strcat(old_commit_address, "/commits/");
            file = fopen(branch_id_address, "r");
            if (file == NULL)
                return 1;
            char *last_com_id = (char *) malloc(10000 * sizeof(char));
            fgets(last_com_id, 10000, file);
            if (last_com_id[strlen(last_com_id) - 1] == '\n')
                last_com_id[strlen(last_com_id) - 1] = '\0';
            char *toke = strtok(last_com_id, " ");
            strcat(old_commit_address, toke);
            fclose(file);

//            char *idstring = (char *) malloc(10000 * sizeof(char));
//            sprintf(idstring, "%d", id - 1);
//            strcat(old_commit_address, str_id);
            dir = opendir(old_commit_address);
            if (dir == NULL) {
                return 1;
            }
            while ((entry = readdir(dir)) != NULL) {
                if (entry->d_name[0] != '.') {
                    char *i_dont_know = (char *) malloc(10000 * sizeof(char));
                    strcpy(i_dont_know, old_commit_address);
                    strcat(i_dont_know, "/");
                    strcat(i_dont_know, entry->d_name);
                    char command[10000] = "";
                    sprintf(command, "rsync -r %s %s", i_dont_know, commit_address);
                    system(command);
                }
            }
            closedir(dir);
        }


        file = fopen(branch_id_address, "r");
        if (file == NULL)
            return 1;
        char *branch_id_address2 = (char *) malloc(100 * sizeof(char));
        strcpy(branch_id_address2, branch_id_address);
        strcat(branch_id_address2, "2");

        file2 = fopen(branch_id_address2, "w");
        fprintf(file2, "%s ", str_id);
        fprintf(file2, "%s\n", commit_address);
        //printf("commit add = %s\n" , commit_address);

        char *id_branch = (char *) malloc(100 * sizeof(char));
        while (fgets(id_branch, 100, file) != NULL) {
            if (id_branch[strlen(id_branch) - 1] == '\n')
                id_branch[strlen(id_branch) - 1] = '\0';
            fprintf(file2, "%s\n", id_branch);
        }
        fclose(file);
        remove(branch_id_address);
        fclose(file2);
        rename(branch_id_address2, branch_id_address);


        file = fopen(".neogit/current_commit_id", "w");
        fprintf(file, "%d %s\n", id + 1, commit_address);///////////////////////id++?
        fclose(file);


        file = fopen(".neogit/commits_id", "w");
        fprintf(file, "%d\n", id + 1);///////////////////////id++?
        fclose(file);
        //if (mkdir(".neogit/master_branche/commits", 0755) != 0) return 1;
        dir = opendir(".neogit/staging");//////????????????
        if (dir == NULL) {
            perror("Error opening staging directory");
            return 1;
        }
        int n_file = 0;
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_name[0] != '.') {////////////stage hidden?
                char *address = (char *) malloc(10000 * sizeof(char));
                strcpy(address, ".neogit/staging/");
                strcat(address, entry->d_name);
                char command[10000] = "";
                sprintf(command, "rsync -r %s %s", address, commit_address);
                system(command);
                char command2[10000] = "";
                sprintf(command2, "rm -r %s", address);
                system(command2);
                n_file++;
            }
        }
        closedir(dir);

        if (id > 1)
            file = fopen(".neogit/commit_info", "r");

        char *file_content = (char *) malloc(10000 * sizeof(char));
        file2 = fopen(".neogit/commit_info2", "w");

        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        fprintf(file2, "%d-%02d-%02d %02d:%02d:%02d ", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour,
                tm.tm_min, tm.tm_sec);
        char *user_name = (char *) malloc(10000 * sizeof(char));
        FILE *config_user = fopen(".neogit/config", "r");
        fgets(user_name, 10000, config_user);
        if (user_name[strlen(user_name) - 1] == '\n')
            user_name[strlen(user_name) - 1] = '\0';
        fclose(config_user);
        char *branch = (char *) malloc(10000 * sizeof(char));
        FILE *br = fopen(".neogit/branch_name", "r");
        fgets(branch, 10000, br);
        if (branch[strlen(branch) - 1] == '\n')
            branch[strlen(branch) - 1] = '\0';
        fclose(br);
        fprintf(file2, "%s %d %s %d $%s\n", user_name, id, branch, n_file, message);/////????????????????
        if (id > 1) {
            while (fgets(file_content, 10000, file) != NULL) {
                //printf("%s\n", file_content);
                if (file_content[strlen(file_content) - 1] == '\n')
                    file_content[strlen(file_content) - 1] = '\0';
                fprintf(file2, "%s\n", file_content);
            }
        }
        if (id > 1) {
            fclose(file);
            remove(".neogit/commit_info");
        }
        fclose(file2);
        rename(".neogit/commit_info2", ".neogit/commit_info");
        printf("%d ", id);
        printf("%d-%02d-%02d %02d:%02d:%02d ", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour,
               tm.tm_min, tm.tm_sec);
        printf("%s\n", message);
        return 0;
    }
}

/////////////////////////////////////////////////////////////////////////////////////

int run_log(int argc, char *const argv[]) {
    if (argc == 2) {
        FILE *file = fopen(".neogit/commit_info", "r");
        char *file_content = (char *) malloc(10000 * sizeof(char));
        while (fgets(file_content, 10000, file) != NULL) {
            //printf("%s\n", file_content);
            if (file_content[strlen(file_content) - 1] == '\n')
                file_content[strlen(file_content) - 1] = '\0';
            printf("%s\n", file_content);
        }
        fclose(file);
        return 0;
    } else if (argc == 4) {
        if (strcmp(argv[2], "-n") == 0) {
            int num;
            num = atoi(argv[3]);
            if (num < 1) {
                printf("invalid command\n");
                return 1;
            } else {
                FILE *file = fopen(".neogit/commit_info", "r");
                char *file_content = (char *) malloc(10000 * sizeof(char));
                for (int i = 0; i < num; i++) {
                    if (fgets(file_content, 10000, file) == NULL) {
                        printf("invalid command\n");
                        return 1;
                    }/////////////////??????????????????????????????????????????????????????
                    //printf("%s\n", file_content);
                    if (file_content[strlen(file_content) - 1] == '\n')
                        file_content[strlen(file_content) - 1] = '\0';
                    printf("%s\n", file_content);
                }
                fclose(file);
                return 0;
            }
        } else if (strcmp(argv[2], "-branch") == 0) {
            int flag = 0;
            FILE *file = fopen(".neogit/branch_name", "r");
            char *file_content = (char *) malloc(10000 * sizeof(char));
            while (fgets(file_content, 10000, file) != NULL) {
                //printf("%s\n", file_content);
                if (file_content[strlen(file_content) - 1] == '\n')
                    file_content[strlen(file_content) - 1] = '\0';
                if (strcmp(argv[3], file_content) == 0) {
                    flag = 1;
                    break;
                }
            }
            fclose(file);
            if (flag == 0) {
                printf("there is not a branch with this name\n");
                return 1;
            }
            flag = 0;
            file = fopen(".neogit/commit_info", "r");
            while (fgets(file_content, 10000, file) != NULL) {
                //printf("%s\n", file_content);
                if (file_content[strlen(file_content) - 1] == '\n')
                    file_content[strlen(file_content) - 1] = '\0';
                char *file_token = (char *) malloc(10000 * sizeof(char));
                strcpy(file_token, file_content);
                char *token = strtok(file_token, " ");
                for (int i = 0; i < 4; i++) {
                    token = strtok(NULL, " ");
                }
                //printf("%s %s\n" , token , file_content);
                if (strcmp(token, argv[3]) == 0) {
                    flag = 1;
                    printf("%s\n", file_content);
                }
            }
            if (flag == 0) {
                printf("nothing has been committed in this branch\n");
                return 1;
            }
            fclose(file);
            return 0;
        } else if (strcmp(argv[2], "-author") == 0) {
            int flag = 0;
            FILE *file = fopen(".neogit/commit_info", "r");
            char *file_content = (char *) malloc(10000 * sizeof(char));
            while (fgets(file_content, 10000, file) != NULL) {
                //printf("%s\n", file_content);
                if (file_content[strlen(file_content) - 1] == '\n')
                    file_content[strlen(file_content) - 1] = '\0';
                char *file_token = (char *) malloc(10000 * sizeof(char));
                strcpy(file_token, file_content);
                char *token = strtok(file_token, " ");
                for (int i = 0; i < 2; i++) {
                    token = strtok(NULL, " ");
                }
                //printf("%s %s\n" , token , file_content);
                if (strcmp(token, argv[3]) == 0) {
                    flag = 1;
                    printf("%s\n", file_content);
                }
            }
            if (flag == 0) {
                printf("this author hasn't committed anything\n");
                return 1;
            }
            fclose(file);
            return 0;
        } else if (strcmp(argv[2], "-since") == 0) {
            int flag = 0;
            FILE *file = fopen(".neogit/commit_info", "r");
            char *file_content = (char *) malloc(10000 * sizeof(char));
            while (fgets(file_content, 10000, file) != NULL) {
                //printf("%s\n", file_content);
                if (file_content[strlen(file_content) - 1] == '\n')
                    file_content[strlen(file_content) - 1] = '\0';
                char *file_token = (char *) malloc(10000 * sizeof(char));
                strcpy(file_token, file_content);
                char *token = strtok(file_token, " ");
                //printf("%s %s\n" , token , file_content);
                char *time = (char *) malloc(10000 * sizeof(char));
                strcpy(time, argv[3]);
                //printf("%d\n" , check_time(time, token));
                if (check_time(time, token) == 1) {
                    flag = 1;
                    printf("%s\n", file_content);
                }
                //printf("%s\n", file_content);
            }
            //printf("100\n");
            if (flag == 0) {
                printf("there hasn't been any commit since this date?\n");
                return 1;
            }
            fclose(file);
            return 0;
        } else if (strcmp(argv[2], "-before") == 0) {
            int flag = 0;
            FILE *file = fopen(".neogit/commit_info", "r");
            char *file_content = (char *) malloc(10000 * sizeof(char));
            while (fgets(file_content, 10000, file) != NULL) {
                //printf("%s\n", file_content);
                if (file_content[strlen(file_content) - 1] == '\n')
                    file_content[strlen(file_content) - 1] = '\0';
                char *file_token = (char *) malloc(10000 * sizeof(char));
                strcpy(file_token, file_content);
                char *token = strtok(file_token, " ");
                //printf("%s %s\n" , token , file_content);
                char *time = (char *) malloc(10000 * sizeof(char));
                strcpy(time, argv[3]);
                if (check_time(time, token) == 0) {
                    flag = 1;
                    printf("%s\n", file_content);
                }
            }
            if (flag == 0) {
                printf("there hasn't been any commit before this date?\n");
                return 1;
            }
            fclose(file);
            return 0;
        } else if (strcmp(argv[2], "-search") == 0) {
            int flag = 0;
            FILE *file = fopen(".neogit/commit_info", "r");
            char *file_content = (char *) malloc(10000 * sizeof(char));
            while (fgets(file_content, 10000, file) != NULL) {
                //printf("%s\n", file_content);
                if (file_content[strlen(file_content) - 1] == '\n')
                    file_content[strlen(file_content) - 1] = '\0';
                char *file_content2 = (char *) malloc(10000 * sizeof(char));
                strcpy(file_content2, file_content);
                file_content2 = strrchr(file_content2, '$');
                for (int i = 0; i < strlen(file_content2) - 1; i++) {
                    file_content2[i] = file_content2[i + 1];
                }
                file_content2[strlen(file_content2) - 1] = '\0';
                char *file_content3 = (char *) malloc(10000 * sizeof(char));
                strcpy(file_content3, file_content2);
                char *token = strtok(file_content3, " ");
                while (token != NULL) {
                    if (strcmp(token, argv[3]) == 0) {
                        flag = 1;
                        printf("%s\n", file_content);
                    }
                    token = strtok(NULL, " ");
                }
            }
            if (flag == 0) {
                printf("there is not a massage including this word\n");
                return 1;
            }
            fclose(file);
            return 0;
        } else
            printf("invalid command\n");
    } else
        printf("invalid command\n");
}

/////////////////////////////////////////////////////////////////////////////////////

int run_branch(int argc, char *const argv[]) {
    if (argc == 2) {
        FILE *file = fopen(".neogit/all_branches", "r");
        char *all_branches_name = (char *) malloc(10000 * sizeof(char));
        while (fgets(all_branches_name, 10000, file) != NULL) {
            if (all_branches_name[strlen(all_branches_name) - 1] == '\n')
                all_branches_name[strlen(all_branches_name) - 1] = '\0';
            printf("%s\n", all_branches_name);
        }
        fclose(file);

    } else if (argc == 3) {

        char *new_branch = (char *) malloc(10000 * sizeof(char));
        strcpy(new_branch, argv[2]);
        //check if it already exists
        FILE *file = fopen(".neogit/all_branches", "r");
        char *all_branches_name = (char *) malloc(10000 * sizeof(char));
        while (fgets(all_branches_name, 10000, file) != NULL) {
            if (all_branches_name[strlen(all_branches_name) - 1] == '\n')
                all_branches_name[strlen(all_branches_name) - 1] = '\0';
            if (strcmp(all_branches_name, new_branch) == 0) {
                printf("there already is a branch with this name\n");
                //return 1;
            }
        }
        fclose(file);

        //adding new branch
        file = fopen(".neogit/all_branches", "a");
        fprintf(file, "%s\n", new_branch);
        fclose(file);

        //making the new branch , commits dir and id file
        char *new_branch_add = (char *) malloc(10000 * sizeof(char));
        strcpy(new_branch_add, ".neogit/branches/");
        strcat(new_branch_add, new_branch);
        if (mkdir(new_branch_add, 0755) != 0) return 1;
        char *new_branch_add2 = (char *) malloc(10000 * sizeof(char));
        strcpy(new_branch_add2, new_branch_add);
        strcat(new_branch_add, "/commits");
        if (mkdir(new_branch_add, 0755) != 0) return 1;
        strcat(new_branch_add2, "/this_branch_id");
        file = fopen(new_branch_add2, "w");
        fprintf(file, "%s\n", "0");
        fclose(file);


        //copy commit
        //search for the last branch /////////////////////////////function?
        file = fopen(".neogit/branch_name", "r");
        if (file == NULL)
            return 1;
        char *last_branch = (char *) malloc(10000 * sizeof(char));
        fgets(last_branch, 10000, file);
        if (last_branch[strlen(last_branch) - 1] == '\n')
            last_branch[strlen(last_branch) - 1] = '\0';
        fclose(file);

        //last branch commit id and commit
        char *last_branch_address = (char *) malloc(10000 * sizeof(char));
        strcpy(last_branch_address, ".neogit/branches/");
        strcat(last_branch_address, last_branch);
        strcat(last_branch_address, "/this_branch_id");
        char *last_branch_id = (char *) malloc(10000 * sizeof(char));
        file = fopen(last_branch_address, "r");
        fgets(last_branch_id, 10000, file);
        if (last_branch_id[strlen(last_branch_id) - 1] == '\n')
            last_branch_id[strlen(last_branch_id) - 1] = '\0';
        char *last_b_id = (char *) malloc(10000 * sizeof(char));
        char *token = strtok(last_branch_id, " ");///////?///////////
        strcpy(last_b_id, token);
        token = strtok(NULL, " ");

        //copy commit
        char command[10000] = "";
        sprintf(command, "rsync -r %s %s", token, new_branch_add);
        system(command);
        //id and new address in new branch id
        char *new_commit_add = (char *) malloc(10000 * sizeof(char));
        strcpy(new_commit_add, new_branch_add);
        file = fopen(new_branch_add2, "w");
        fprintf(file, "%s ", last_b_id);
        strcat(new_commit_add, "/");
        strcat(new_commit_add, last_b_id);
        fprintf(file, "%s\n", new_commit_add);
        fprintf(file, "%s\n", "0");
        fclose(file);

    } else {
        printf("invalid command\n");
        return 1;
    }
}

/////////////////////////////////////////////////////////////////////////////////////

int run_checkout(int argc, char *const argv[]) {
    if (argc == 3) {
        if (strcmp(argv[2], "HEAD") == 0) {


            //if repository and last commit are same

            //the last branch
            FILE *file = fopen(".neogit/branch_name", "r");
            if (file == NULL)
                return 1;
            char *last_branch = (char *) malloc(10000 * sizeof(char));
            fgets(last_branch, 10000, file);
            if (last_branch[strlen(last_branch) - 1] == '\n')
                last_branch[strlen(last_branch) - 1] = '\0';
            fclose(file);

            //last commit
            char *last_branch_to_id = (char *) malloc(10000 * sizeof(char));
            strcpy(last_branch_to_id, ".neogit/branches/");
            strcat(last_branch_to_id, last_branch);
            strcpy(last_branch_to_id, "/this_branch_id");
            char *last_commit = (char *) malloc(10000 * sizeof(char));
            file = fopen(last_branch_to_id, "r");/////////////////////close
            fgets(last_commit, 10000, file);
            if (last_commit[strlen(last_commit) - 1] == '\n')
                last_commit[strlen(last_commit) - 1] = '\0';
            char *last_id = (char *) malloc(10000 * sizeof(char));
            char *token = strtok(last_commit, " ");
            strcpy(last_id, token);
            char *last_commit_address = (char *) malloc(10000 * sizeof(char));
            token = strtok(NULL, " ");
            strcpy(last_commit_address, token);
            fclose(file);

            //compare
            int counter = 0;
            int counter2 = 0;
            int counter3 = 0;
            struct dirent *entry;
            DIR *dir = opendir(".");
            if (dir == NULL) {
                perror("Error opening current directory");
                return 1;
            }

            struct dirent *entry_commit;
            DIR *dir_commit = opendir(last_commit_address);
            if (dir_commit == NULL) {
                perror("Error opening current directory");
                return 1;
            }

            while ((entry = readdir(dir)) != NULL) {
                if (entry->d_name[0] != '.') {
                    counter++;
                    while ((entry_commit = readdir(dir_commit)) != NULL) {
                        if (entry_commit->d_name[0] != '.') {
                            counter3++;
                            if (strcmp(entry_commit->d_name, entry->d_name) == 0) {
                                counter2++;
                            }
                        }
                    }
                }
            }
            closedir(dir);
            closedir(dir_commit);

            //check
            if (counter != counter2) {
                printf("files in repository has not been commited yet\n");
                return 1;
            }
            if (counter3 != counter2 * counter2) {
                printf("files in repository has not been commited yet\n");
                return 1;
            }


            return 0;
        } else if (strcmp(argv[2], "HEAD-n") == 0) {

            return 0;
        } else {

            //if it is number
            int flag = 0;
            char *sth = (char *) malloc(10000 * sizeof(char));
            strcpy(sth, argv[2]);
            for (int i = 0; i < strlen(sth); i++) {
                if ((sth[i] < 48) || (sth[i] > 57)) {
                    flag = 1;
                    break;
                }
            }
            if (flag == 1) {

                //check if the branch already exists
                flag = 0;
                char *new_branch = (char *) malloc(10000 * sizeof(char));
                strcpy(new_branch, argv[2]);
                FILE *file = fopen(".neogit/all_branches", "r");
                char *all_branches_name = (char *) malloc(10000 * sizeof(char));
                while (fgets(all_branches_name, 10000, file) != NULL) {
                    if (all_branches_name[strlen(all_branches_name) - 1] == '\n')
                        all_branches_name[strlen(all_branches_name) - 1] = '\0';
                    if (strcmp(all_branches_name, new_branch) == 0) {
                        flag = 1;
                    }
                }
                fclose(file);
                if (flag == 0) {
                    printf("there is no branch with this name\n");
                    return 1;
                }


                //if repository and last commit are same
                //the last branch
                file = fopen(".neogit/branch_name", "r");
                if (file == NULL)
                    return 1;
                char *last_branch = (char *) malloc(10000 * sizeof(char));
                fgets(last_branch, 10000, file);
                if (last_branch[strlen(last_branch) - 1] == '\n')
                    last_branch[strlen(last_branch) - 1] = '\0';
                fclose(file);
                //last commit
                char *last_branch_to_id = (char *) malloc(10000 * sizeof(char));
                strcpy(last_branch_to_id, ".neogit/branches/");
                strcat(last_branch_to_id, last_branch);
                strcat(last_branch_to_id, "/this_branch_id");
                char *last_commit = (char *) malloc(10000 * sizeof(char));
                file = fopen(last_branch_to_id, "r");/////////////////////close
                fgets(last_commit, 10000, file);
                if (last_commit[strlen(last_commit) - 1] == '\n')
                    last_commit[strlen(last_commit) - 1] = '\0';
                char *last_id = (char *) malloc(10000 * sizeof(char));
                char *token = strtok(last_commit, " ");
                strcpy(last_id, token);
                char *last_commit_address = (char *) malloc(10000 * sizeof(char));
                token = strtok(NULL, " ");
                strcpy(last_commit_address, token);
                fclose(file);

                //current id
                file = fopen(".neogit/commits_id", "w");
                fprintf(file, "%s %s\n", last_id, last_commit_address);///////////////////////id++?
                fclose(file);

                //compare
                int counter = 0;
                int counter2 = 0;
                int counter3 = 0;
                struct dirent *entry;
                DIR *dir = opendir(".");
                if (dir == NULL) {
                    perror("Error opening current directory");
                    return 1;
                }
                struct dirent *entry_commit;
                DIR *dir_commit;

                while ((entry = readdir(dir)) != NULL) {
                    if (entry->d_name[0] != '.') {
                        counter++;
                        dir_commit = opendir(last_commit_address);
                        if (dir_commit == NULL) {
                            perror("Error opening current directory");
                            return 1;
                        }
                        while ((entry_commit = readdir(dir_commit)) != NULL) {
                            if (entry_commit->d_name[0] != '.') {
                                counter3++;
                                if (strcmp(entry_commit->d_name, entry->d_name) == 0) {
                                    counter2++;
                                }
                            }
                        }
                        closedir(dir_commit);
                    }
                }
                closedir(dir);

                //check
                if (counter != counter2) {
                    printf("files in repository has not been commited yet\n");
                    return 1;
                }
                if (counter3 != counter2 * counter2) {
                    printf("files in repository has not been commited yet\n");
                    return 1;
                }

                //move to the new branch
                //remove files and directories
                dir = opendir(".");
                if (dir == NULL) {
                    perror("Error opening current directory");
                    return 1;
                }
                while ((entry = readdir(dir)) != NULL) {
                    if (entry->d_name[0] != '.') {
                        char command[100];
                        sprintf(command, "rm -r %s", entry->d_name);
                        system(command);
                    }
                }
                closedir(dir);

                //new branch address
                char *new_branch_to_id = (char *) malloc(10000 * sizeof(char));
                strcpy(new_branch_to_id, ".neogit/branches/");
                strcat(new_branch_to_id, new_branch);
                strcat(new_branch_to_id, "/this_branch_id");
                char *last_commit_in_new_branch = (char *) malloc(10000 * sizeof(char));
                file = fopen(new_branch_to_id, "r");/////////////////////close
                fgets(last_commit_in_new_branch, 10000, file);
                if (last_commit_in_new_branch[strlen(last_commit_in_new_branch) - 1] == '\n')
                    last_commit_in_new_branch[strlen(last_commit_in_new_branch) - 1] = '\0';
                char *last_id_nbranch = (char *) malloc(10000 * sizeof(char));
                char *tok = strtok(last_commit_in_new_branch, " ");
                strcpy(last_id_nbranch, tok);///????????????????????????????????????????
                char *last_commit_nbranch_address = (char *) malloc(10000 * sizeof(char));
                tok = strtok(NULL, " ");
                strcpy(last_commit_nbranch_address, tok);
                fclose(file);

                //open dir & copy
                char *new_rep_file = (char *) malloc(10000 * sizeof(char));
                dir = opendir(last_commit_nbranch_address);
                if (dir == NULL) {
                    perror("Error opening current directory");
                    return 1;
                }
                while ((entry = readdir(dir)) != NULL) {
                    if (entry->d_name[0] != '.') {
                        strcpy(new_rep_file, last_commit_nbranch_address);
                        strcat(new_rep_file, "/");
                        strcat(new_rep_file, entry->d_name);
                        char command[10000] = "";
                        sprintf(command, "rsync -r %s %s", new_rep_file, ".");
                        system(command);
                    }
                }
                closedir(dir);

                //change last branch
                file = fopen(".neogit/branch_name", "w");
                fprintf(file, "%s\n", new_branch);
                fclose(file);

                return 0;
            } else {
                //if id is valid ??????
                int id;
                id = atoi(sth);
                char *last_id1 = (char *) malloc(100 * sizeof(char));
                FILE *file = fopen(".neogit/commits_addressandid", "r");
                char *last_commit1 = (char *) malloc(10000 * sizeof(char));
                fgets(last_commit1, 10000, file);
                if (last_commit1[strlen(last_commit1) - 1] == '\n')
                    last_commit1[strlen(last_commit1) - 1] = '\0';
                //char *last_id_nbranch = (char *) malloc(10000 * sizeof(char));
                char *tok = strtok(last_commit1, " ");
                strcpy(last_id1, tok);///????????????????????????????????????????
                char *last_address = (char *) malloc(10000 * sizeof(char));
                tok = strtok(NULL, " ");
                strcpy(last_address, tok);
                int id_last = atoi(last_id1);
                //id_last--;
                if (id > id_last) {
                    printf("invalid id\n");
                    return 1;
                }
                fclose(file);

                //if repository and last commit are same

                //the last branch
                file = fopen(".neogit/branch_name", "r");
                if (file == NULL)
                    return 1;
                char *last_branch = (char *) malloc(10000 * sizeof(char));
                fgets(last_branch, 10000, file);
                if (last_branch[strlen(last_branch) - 1] == '\n')
                    last_branch[strlen(last_branch) - 1] = '\0';
                fclose(file);

                //last commit
                char *last_branch_to_id = (char *) malloc(10000 * sizeof(char));
                strcpy(last_branch_to_id, ".neogit/branches/");
                strcat(last_branch_to_id, last_branch);
                strcat(last_branch_to_id, "/this_branch_id");
                char *last_commit = (char *) malloc(10000 * sizeof(char));
                file = fopen(last_branch_to_id, "r");/////////////////////close
                fgets(last_commit, 10000, file);
                if (last_commit[strlen(last_commit) - 1] == '\n')
                    last_commit[strlen(last_commit) - 1] = '\0';
                char *last_id = (char *) malloc(10000 * sizeof(char));
                char *token = strtok(last_commit, " ");
                strcpy(last_id, token);
                char *last_commit_address = (char *) malloc(10000 * sizeof(char));
                token = strtok(NULL, " ");
                strcpy(last_commit_address, token);
                fclose(file);

                //compare
                int counter = 0;
                int counter2 = 0;
                int counter3 = 0;
                struct dirent *entry;
                DIR *dir = opendir(".");
                if (dir == NULL) {
                    perror("Error opening current directory");
                    return 1;
                }

                struct dirent *entry_commit;
                DIR *dir_commit;

                while ((entry = readdir(dir)) != NULL) {
                    if (entry->d_name[0] != '.') {
                        counter++;
                        dir_commit = opendir(last_commit_address);
                        if (dir_commit == NULL) {
                            perror("Error opening current directory");
                            return 1;
                        }
                        while ((entry_commit = readdir(dir_commit)) != NULL) {
                            if (entry_commit->d_name[0] != '.') {
                                counter3++;
                                if (strcmp(entry_commit->d_name, entry->d_name) == 0) {
                                    counter2++;
                                }
                            }
                        }
                        closedir(dir_commit);
                    }
                }
                closedir(dir);


                //check
                if (counter != counter2) {
                    printf("files in repository has not been commited yet\n");
                    return 1;
                }
//                if (counter3 != counter2 * counter2) {
//                    printf("files in repository has not been commited yet\n");
//                    return 1;
//                }

                //copy

                //go to new commit
//                char *new_rep_file = (char *) malloc(10000 * sizeof(char));
//                dir = opendir(last_address);
//                if (dir == NULL) {
//                    perror("Error opening current directory");
//                    return 1;
//                }
//                while ((entry = readdir(dir)) != NULL) {
//                    if (entry->d_name[0] != '.') {
//                        strcpy(new_rep_file, last_address);
//                        strcat(new_rep_file, "/");
//                        strcat(new_rep_file, entry->d_name);
//                        char command[10000] = "";
//                        sprintf(command, "rsync -r %s %s", new_rep_file, ".");
//                        system(command);
//                    }
//                }
//                closedir(dir);

                //change branch?????????????
                char *which_branch = strtok(last_address, "/");
                for (int i = 0; i < 2; i++) {
                    which_branch = strtok(NULL, "/");
                }
                if (strcmp(which_branch, last_branch) != 0) {
                    file = fopen(".neogit/branch_name", "w");
                    fprintf(file, "%s\n", which_branch);
                    fclose(file);
                }

                return 0;


            }

        }
    } else {
        printf("invalid command\n");
        return 1;
    }
}

///////////////////////////////////////////////////////////////////////////////////

int run_status(int argc, char *const argv[]) {
    if (argc > 2) {
        printf("invalid commann\n");
        return 1;
    } else {

        //find last commit
        char *last_id = (char *) malloc(100 * sizeof(char));
        FILE *file = fopen(".neogit/commits_addressandid", "r");
        char *last_commit = (char *) malloc(10000 * sizeof(char));
        fgets(last_commit, 10000, file);
        if (last_commit[strlen(last_commit) - 1] == '\n')
            last_commit[strlen(last_commit) - 1] = '\0';
        //char *last_id_nbranch = (char *) malloc(10000 * sizeof(char));
        char *tok = strtok(last_commit, " ");
        strcpy(last_id, tok);///????????????????????????????????????????
        char *last_address = (char *) malloc(10000 * sizeof(char));
        tok = strtok(NULL, " ");
        strcpy(last_address, tok);
        fclose(file);

        //open it
        int counter = 0;
        int counter2 = 0;
        int counter3 = 0;
        int flag = -1;
        int flag2 = -1;
        int flag3 = -1;
        struct dirent *entry;
        DIR *dir = opendir(".");
        if (dir == NULL) {
            perror("Error opening current directory");
            return 1;
        }

        struct dirent *entry_commit;
        DIR *dir_commit;


        struct dirent *entry_stage;
        DIR *dir_stage;

        //printf("1\n");
        FILE *file2;
        char *file_address_repo = (char *) malloc(10000 * sizeof(char));
        char *file_address = (char *) malloc(10000 * sizeof(char));
        char *repo_file_content = (char *) malloc(10000 * sizeof(char));
        char *commit_file_content = (char *) malloc(10000 * sizeof(char));
        //printf("1\n");
        while ((entry = readdir(dir)) != NULL) {
            //printf("1\n");
            if (entry->d_name[0] != '.' && entry->d_type == DT_REG) {
                flag = 0;
                flag2 = 0;
                dir_stage = opendir(".neogit/staging");
                if (dir_stage == NULL) {
                    perror("Error opening current directory");
                    return 1;
                }
                while ((entry_stage = readdir(dir_stage)) != NULL) {
                    if (entry_stage->d_name[0] != '.' && entry_stage->d_type == DT_REG) {
                        if (strcmp(entry_stage->d_name, entry->d_name) == 0) {
                            flag = 1;
                        }
                    }
                }
                closedir(dir_stage);
                dir_commit = opendir(last_address);
                if (dir_commit == NULL) {
                    perror("Error opening current directory");
                    return 1;
                }
                while ((entry_commit = readdir(dir_commit)) != NULL) {
                    if (entry_commit->d_name[0] != '.' && entry_commit->d_type == DT_REG) {
                        if (strcmp(entry_commit->d_name, entry->d_name) == 0) {
                            flag2 = 1;
                            strcpy(file_address_repo, ".neogit/");
                            strcat(file_address_repo, entry->d_name);
                            file = fopen(entry->d_name, "r");
                            //printf("%s\n" , entry->d_name);//close
                            if (file == NULL) return 1;
                            strcpy(file_address, last_address);
                            strcat(file_address, "/");
                            strcat(file_address, entry_commit->d_name);
                            char cwd[100];
                            getcwd(cwd, sizeof(cwd));
                            file2 = fopen(file_address, "r");
                            if (file2 == NULL) return 1;
                            fscanf(file, "%[^\r]s", repo_file_content);
                            fscanf(file2, "%[^\r]s", commit_file_content);
                            fclose(file);
                            fclose(file2);
                            if (strcmp(commit_file_content, repo_file_content) != 0) {
                                if (flag == 1)
                                    printf("%s %c%c\n", entry->d_name, '+', 'M');
                                else
                                    printf("%s %c%c\n", entry->d_name, '-', 'M');
                                break;
                            }
                        }
                    }

                }
                if (flag2 == 0) {
                    if (flag == 1)
                        printf("%s %c%c\n", entry->d_name, '+', 'A');
                    else
                        printf("%s %c%c\n", entry->d_name, '-', 'A');
                }
            }
        }
        closedir(dir);

        //if removed


        dir_commit = opendir(last_address);
        if (dir_commit == NULL) {
            perror("Error opening current directory");
            return 1;
        }

        flag = 0;
        flag2 = 0;
        while ((entry_commit = readdir(dir_commit)) != NULL) {
            if (entry_commit->d_name[0] != '.' && entry_commit->d_type == DT_REG) {
                flag = 0;
                flag2 = 0;
                dir = opendir(".");
                if (dir == NULL) {
                    perror("Error opening current directory");
                    return 1;
                }
                while ((entry = readdir(dir)) != NULL) {
                    if (entry->d_name[0] != '.' && entry->d_type == DT_REG) {
                        if (strcmp(entry->d_name, entry_commit->d_name) == 0) {
                            flag = 1;
                        }
                    }
                }
                closedir(dir);
                dir_stage = opendir(".neogit/staging");
                if (dir_stage == NULL) {
                    perror("Error opening current directory");
                    return 1;
                }
                while ((entry_stage = readdir(dir_stage)) != NULL) {
                    if (entry_stage->d_name[0] != '.' && entry_stage->d_type == DT_REG) {
                        if (strcmp(entry_stage->d_name, entry_commit->d_name) == 0) {
                            flag2 = 1;
                            if (flag == 0)
                                printf("%s %c%c\n", entry_commit->d_name, '+', 'D');
                            break;
                        }
                    }
                }
                closedir(dir_stage);
                if (flag2 == 0 && flag == 0) {
                    printf("%s %c%c\n", entry_commit->d_name, '-', 'D');
                }
            }
        }
        closedir(dir_commit);
    }
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////

int run_set(int argc, char *const argv[]) {
    if (argc != 6) {
        printf("invalid command\n");
        return 1;
    }
    if ((strcmp(argv[2], "-m") != 0) || (strcmp(argv[4], "-s") != 0)) {
        printf("invalid command\n");
        return 1;
    }
    if (strlen(argv[3]) > 72) {
        printf("your commit message has too many characters\n");
        return 1;
    }
    //open shortcut file
    FILE *file = fopen(".neogit/commit_shortcut", "a");

    //write and shortcut
    fprintf(file, "%s\n", argv[5]);
    fprintf(file, "%s\n", argv[3]);
    fclose(file);
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////

int run_replace(int argc, char *const argv[]) {
    if (argc != 6) {////??
        printf("invalid command\n");
        return 1;
    }
    if ((strcmp(argv[2], "-m") != 0) || (strcmp(argv[4], "-s") != 0)) {
        printf("invalid command\n");
        return 1;
    }

    //open shortcut file
    FILE *file = fopen(".neogit/commit_shortcut", "r");

    //find shortcut
    int counter = 0;
    int counter2 = -1;
    char *shortcut = (char *) malloc(10000 * sizeof(char));
    while (fgets(shortcut, 10000, file) != NULL) {
        if (shortcut[strlen(shortcut) - 1] == '\n')
            shortcut[strlen(shortcut) - 1] = '\0';
        counter++;
        if (strcmp(shortcut, argv[5]) == 0) {
            counter2 = counter + 1;
            break;
        }
    }
    fclose(file);\
    if (counter2 == -1) {
        printf("the shortcut doesnt exist\n");
        return 1;
    }
    //replace
    file = fopen(".neogit/commit_shortcut", "r");
    FILE *file2 = fopen(".neogit/commit_shortcut2", "w");
    char *new_shortcut_message = (char *) malloc(10000 * sizeof(char));
    strcpy(new_shortcut_message, argv[3]);
    for (int i = 0; i < counter2 - 1; i++) {
        fgets(shortcut, 10000, file);
        if (shortcut[strlen(shortcut) - 1] == '\n')
            shortcut[strlen(shortcut) - 1] = '\0';
        fprintf(file2, "%s\n", shortcut);
    }
    fprintf(file2, "%s\n", new_shortcut_message);
    fgets(shortcut, 10000, file);
    if (shortcut[strlen(shortcut) - 1] == '\n')
        shortcut[strlen(shortcut) - 1] = '\0';
    for (int i = counter2; i < counter - 1; i++) {
        fgets(shortcut, 10000, file);
        if (shortcut[strlen(shortcut) - 1] == '\n')
            shortcut[strlen(shortcut) - 1] = '\0';
        fprintf(file2, "%s\n", shortcut);
    }
    fclose(file);
    remove(".neogit/commit_shortcut");
    fclose(file2);
    rename(".neogit/commit_shortcut2", ".neogit/commit_shortcut");
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////

int run_remove(int argc, char *const argv[]) {
    if (argc != 4) {
        printf("invalid command\n");
        return 1;
    }
    if (strcmp(argv[2], "-s") != 0) {
        printf("invalid command\n");
        return 1;
    }
    //open shortcut file
    FILE *file = fopen(".neogit/commit_shortcut", "r");

    //find shortcut
    int counter = 0;
    int counter2 = -1;
    char *shortcut = (char *) malloc(10000 * sizeof(char));
    while (fgets(shortcut, 10000, file) != NULL) {
        if (shortcut[strlen(shortcut) - 1] == '\n')
            shortcut[strlen(shortcut) - 1] = '\0';
        counter++;
        if (strcmp(shortcut, argv[3]) == 0) {
            counter2 = counter;
            break;
        }
    }
    fclose(file);
    if (counter2 == -1) {
        printf("the shortcut doesnt exist\n");
        return 1;
    }

    //remove
    file = fopen(".neogit/commit_shortcut", "r");
    FILE *file2 = fopen(".neogit/commit_shortcut2", "w");
    for (int i = 0; i < counter2 - 1; i++) {
        fgets(shortcut, 10000, file);
        if (shortcut[strlen(shortcut) - 1] == '\n')
            shortcut[strlen(shortcut) - 1] = '\0';
        fprintf(file2, "%s\n", shortcut);
    }
    fgets(shortcut, 10000, file);
    if (shortcut[strlen(shortcut) - 1] == '\n')
        shortcut[strlen(shortcut) - 1] = '\0';
    fgets(shortcut, 10000, file);
    if (shortcut[strlen(shortcut) - 1] == '\n')
        shortcut[strlen(shortcut) - 1] = '\0';
    for (int i = counter2 + 1; i < counter - 1; i++) {
        fgets(shortcut, 10000, file);
        if (shortcut[strlen(shortcut) - 1] == '\n')
            shortcut[strlen(shortcut) - 1] = '\0';
        fprintf(file2, "%s\n", shortcut);
    }
    fclose(file);
    remove(".neogit/commit_shortcut");
    fclose(file2);
    rename(".neogit/commit_shortcut2", ".neogit/commit_shortcut");
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////

//int run_diff(int argc, char *const argv[]) {
//    if (argc == 5) {
//        if(strcmp(argv[2] , "-f") != 0)
//        {
//            printf("invalid command\n");
//            return 1;
//        }
//        //find file 1
//        char * address1 = (char*)malloc(10000 * sizeof(char));
//        strcpy(address1 , argv[3]);
//        FILE * file1 = fopen(address1 , "r");
//        if(file1 == NULL)
//        {
//            printf("file1 doesnt exist\n");
//            return 1;
//        }
//
//        //find file2
//        char * address2 = (char*)malloc(10000 * sizeof(char));
//        strcpy(address2 , argv[3]);
//        FILE * file2 = fopen(address2 , "r");
//        if(file2 == NULL)
//        {
//            printf("file2 doesnt exist\n");
//            return 1;
//        }
//
//        //compare
//        int line1 = 0;
//        int line2 = 0;
//        char * file1_content = (char*)malloc(10000 * sizeof(char));
//        char * file2_content = (char*)malloc(10000 * sizeof(char));
//        while(fgets(file1_content , 10000 , file1) != NULL){
//            line1++;
//            while(isWhitespaceLine(file1_content) == 1)
//            {
//                fgets(file1_content , 10000 , file1);
//                if(file1_content == NULL)
//                    break;
//                line1++;
//            }
//            fgets(file2_content , 10000 , file2);
//            line2++;
//            while(isWhitespaceLine(file2_content) == 1)
//            {
//                fgets(file2_content , 10000 , file2);
//                line2++;
//            }
//            char * token = strtok(file1 , " ");
//            char * tok = strtok(file2 , " ");
//            if(strcmp(token , tok) != 0)
//            {
//                printf("%s %d\n" , address1 , line1);
//                printf("%s %d\n\n" , address2 , line2);
//            }
//        }
//
//
//
//
//        return 0;
//    } else if (argc == 9) {}
//    else {
//        printf("invalid command\n");
//        return 1;
//    }
//}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

int run_tag(int argc, char *const argv[]) {
    if (argc > 3) {
        if (argc == 4) {
            if (strcmp(argv[2], "-a") == 0) {
                //new tag
                char *new_tag = (char *) malloc(10000 * sizeof(char));
                strcpy(new_tag, argv[3]);

                //check new tag
                char *tags = (char *) malloc(10000 * sizeof(char));
                FILE *file = fopen(".neogit/tags/tag_name", "r");
                while (fgets(tags, 10000, file) != NULL) {
                    if (tags[strlen(tags) - 1] == '\n')
                        tags[strlen(tags) - 1] = '\0';
                    if (strcmp(new_tag, tags) == 0) {
                        printf("this tag already exists\n");
                        return 1;
                    }
                }
                fclose(file);

                //current id
                file = fopen(".neogit/ccurrent_commit_id", "r");
                char *current_id = (char *) malloc(10000 * sizeof(char));
                fgets(current_id, 10000, file);
                if (current_id[strlen(current_id) - 1] == '\n')
                    current_id[strlen(current_id) - 1] = '\0';
                char *token = strtok(current_id, " ");
                strcpy(current_id, token);
                fclose(file);

                //author
                file = fopen(".neogit/config", "r");
                char *user_name = (char *) malloc(10000 * sizeof(char));
                char *email = (char *) malloc(10000 * sizeof(char));
                fgets(user_name, 10000, file);
                if (user_name[strlen(user_name) - 1] == '\n')
                    user_name[strlen(user_name) - 1] = '\0';
                fgets(email, 10000, file);
                if (email[strlen(email) - 1] == '\n')
                    email[strlen(email) - 1] = '\0';

                //time
                time_t cur = time(NULL);
                char cur_time[1000] = "";
                strcpy(cur_time, ctime(&cur));


                //check if its first and write it
                file = fopen(".neogit/tags/tag_name", "r");
                fgets(tags, 10000, file);
                if (tags[strlen(tags) - 1] == '\n')
                    tags[strlen(tags) - 1] = '\0';
                if (strcmp(tags, "0") == 0) {
                    fclose(file);
                    file = fopen(".neogit/tags/tag_name", "w");
                    fprintf(file, "%s\n", new_tag);
                } else {
                    //weite it
                    fclose(file);
                    file = fopen(".neogit/tags/tag_name", "r");
                    FILE *file2 = fopen(".neogit/tags/tag_name2", "w");//close
                    int flag = 0;
                    while (fgets(tags, 10000, file) != NULL) {
                        if (tags[strlen(tags) - 1] == '\n')
                            tags[strlen(tags) - 1] = '\0';
                        unsigned long cn;
                        if (strlen(new_tag) >= strlen(tags)) {
                            cn = strlen(tags);
                        } else {
                            cn = strlen(new_tag);
                        }
                        for (int i = 0; i < cn; i++) {
                            if (tags[i] < new_tag[i]) {
                                fprintf(file2, "%s\n", tags);
                                break;
                            } else if (tags[i] > new_tag[i]) {
                                flag = 1;
                                fprintf(file2, "%s\n", new_tag);
                                break;
                            }
                        }
                    }
                    if (flag == 1) {
                        fprintf(file2, "%s\n", new_tag);
                    }
                    fclose(file);
                    remove(".neogit/tags/tag_name");
                    fclose(file2);
                    rename(".neogit/tags/tag_name2", ".neogit/tags/tag_name");
                }

                //write in data
                file = fopen(".neogit/tags/tag_data", "r");
                char *tag_contant = (char *) malloc(10000 * sizeof(char));
                fgets(tag_contant, 10000, file);
                if (tag_contant[strlen(tag_contant) - 1] == '\n')
                    tag_contant[strlen(tag_contant) - 1] = '\0';
                if (strcmp(tag_contant, "0") == 0) {
                    fclose(file);
                    file = fopen(".neogit/tags/tag_data", "w");
                    fprintf(file, "%s ", new_tag);
                    fprintf(file, "%s ", current_id);
                    fprintf(file, "%s ", user_name);
                    fprintf(file, "%s ", email);
                    fprintf(file, "%s ", cur_time);
                    fprintf(file, "%s\n", "0");
                    return 0;
                } else {
                    fclose(file);
                    file = fopen(".neogit/tags/tag_data", "a");
                    fprintf(file, "%s ", new_tag);
                    fprintf(file, "%s ", current_id);
                    fprintf(file, "%s ", user_name);
                    fprintf(file, "%s ", email);
                    fprintf(file, "%s ", cur_time);
                    fprintf(file, "%s\n", "0");
                    return 0;
                }

            } else if (strcmp(argv[2], "show") == 0) {}
            else {
                printf("invalid command\n");
                return 1;
            }
            return 0;
        } else if (argc == 5) {
            if ((strcmp(argv[2], "-a") == 0) && (strcmp(argv[4], "-f")) == 0) {

                //new tag
                int flag = 0;
                char *new_tag = (char *) malloc(10000 * sizeof(char));
                strcpy(new_tag, argv[3]);

                //check new tag

                char *tags = (char *) malloc(10000 * sizeof(char));
                FILE *file = fopen(".neogit/tags/tag_name", "r");
                while (fgets(tags, 10000, file) != NULL) {
                    if (tags[strlen(tags) - 1] == '\n')
                        tags[strlen(tags) - 1] = '\0';
                    if (strcmp(new_tag, tags) == 0) {
                        flag = 1;
                    }
                }
                fclose(file);

                if (flag == 0) {

                    //current id
                    file = fopen(".neogit/ccurrent_commit_id", "r");
                    char *current_id = (char *) malloc(10000 * sizeof(char));
                    fgets(current_id, 10000, file);
                    if (current_id[strlen(current_id) - 1] == '\n')
                        current_id[strlen(current_id) - 1] = '\0';
                    char *token = strtok(current_id, " ");
                    strcpy(current_id, token);
                    fclose(file);

                    //author
                    file = fopen(".neogit/config", "r");
                    char *user_name = (char *) malloc(10000 * sizeof(char));
                    char *email = (char *) malloc(10000 * sizeof(char));
                    fgets(user_name, 10000, file);
                    if (user_name[strlen(user_name) - 1] == '\n')
                        user_name[strlen(user_name) - 1] = '\0';
                    fgets(email, 10000, file);
                    if (email[strlen(email) - 1] == '\n')
                        email[strlen(email) - 1] = '\0';

                    //time
                    time_t cur = time(NULL);
                    char cur_time[1000] = "";
                    strcpy(cur_time, ctime(&cur));

                    //check if its first and write it
                    file = fopen(".neogit/tags/tag_name", "r");
                    fgets(tags, 10000, file);
                    if (tags[strlen(tags) - 1] == '\n')
                        tags[strlen(tags) - 1] = '\0';
                    if (strcmp(tags, "0") == 0) {
                        fclose(file);
                        file = fopen(".neogit/tags/tag_name", "w");
                        fprintf(file, "%s\n", argv[3]);
                    } else {
                        //write it
                        fclose(file);
                        file = fopen(".neogit/tags/tag_name", "r");
                        FILE *file2 = fopen(".neogit/tags/tag_name2", "w");//close
                        flag = 0;
                        while (fgets(tags, 10000, file) != NULL) {
                            if (tags[strlen(tags) - 1] == '\n')
                                tags[strlen(tags) - 1] = '\0';
                            unsigned long cn;
                            if (strlen(new_tag) >= strlen(tags)) {
                                cn = strlen(tags);
                            } else {
                                cn = strlen(new_tag);
                            }
                            for (int i = 0; i < cn; i++) {
                                if (tags[i] < new_tag[i]) {
                                    fprintf(file2, "%s\n", tags);
                                    break;
                                } else if (tags[i] > new_tag[i]) {
                                    flag = 1;
                                    fprintf(file2, "%s\n", new_tag);
                                    break;
                                }
                            }
                        }
                        if (flag == 1) {
                            fprintf(file2, "%s\n", new_tag);
                        }
                        fclose(file);
                        remove(".neogit/tags/tag_name");
                        fclose(file2);
                        rename(".neogit/tags/tag_name2", ".neogit/tags/tag_name");
                    }

                    //write in data
                    file = fopen(".neogit/tags/tag_data", "r");
                    char *tag_contant = (char *) malloc(10000 * sizeof(char));
                    fgets(tag_contant, 10000, file);
                    if (tag_contant[strlen(tag_contant) - 1] == '\n')
                        tag_contant[strlen(tag_contant) - 1] = '\0';
                    if (strcmp(tag_contant, "0") == 0) {
                        fclose(file);
                        file = fopen(".neogit/tags/tag_data", "w");
                        fprintf(file, "%s ", new_tag);
                        fprintf(file, "%s ", current_id);
                        fprintf(file, "%s ", user_name);
                        fprintf(file, "%s ", email);
                        fprintf(file, "%s ", cur_time);
                        fprintf(file, "%s\n", "0");
                        return 0;
                    } else {
                        fclose(file);
                        file = fopen(".neogit/tags/tag_data", "a");
                        fprintf(file, "%s ", new_tag);
                        fprintf(file, "%s ", current_id);
                        fprintf(file, "%s ", user_name);
                        fprintf(file, "%s ", email);
                        fprintf(file, "%s ", cur_time);
                        fprintf(file, "%s\n", "0");
                        return 0;
                    }
                }
                return 0;

            } else {
                printf("invalid command\n");
                return 1;
            }
        } else if (argc == 6) {
            if (strcmp(argv[2], "-a") == 0) {
                if (strcmp(argv[4], "-m") == 0) {

                    //new tag
                    char *new_tag = (char *) malloc(10000 * sizeof(char));
                    strcpy(new_tag, argv[3]);

                    //check new tag
                    char *tags = (char *) malloc(10000 * sizeof(char));
                    FILE *file = fopen(".neogit/tags/tag_name", "r");
                    while (fgets(tags, 10000, file) != NULL) {
                        if (tags[strlen(tags) - 1] == '\n')
                            tags[strlen(tags) - 1] = '\0';
                        if (strcmp(new_tag, tags) == 0) {
                            printf("this tag already exists\n");
                            return 1;
                        }
                    }
                    fclose(file);

                    //message
                    char *message = (char *) malloc(10000 * sizeof(char));
                    strcpy(message, argv[5]);

                    //current id
                    file = fopen(".neogit/ccurrent_commit_id", "r");
                    char *current_id = (char *) malloc(10000 * sizeof(char));
                    fgets(current_id, 10000, file);
                    if (current_id[strlen(current_id) - 1] == '\n')
                        current_id[strlen(current_id) - 1] = '\0';
                    char *token = strtok(current_id, " ");
                    strcpy(current_id, token);
                    fclose(file);

                    //author
                    file = fopen(".neogit/config", "r");
                    char *user_name = (char *) malloc(10000 * sizeof(char));
                    char *email = (char *) malloc(10000 * sizeof(char));
                    fgets(user_name, 10000, file);
                    if (user_name[strlen(user_name) - 1] == '\n')
                        user_name[strlen(user_name) - 1] = '\0';
                    fgets(email, 10000, file);
                    if (email[strlen(email) - 1] == '\n')
                        email[strlen(email) - 1] = '\0';

                    //time
                    time_t cur = time(NULL);
                    char cur_time[1000] = "";
                    strcpy(cur_time, ctime(&cur));


                    //check if its first and write it
                    file = fopen(".neogit/tags/tag_name", "r");
                    fgets(tags, 10000, file);
                    if (tags[strlen(tags) - 1] == '\n')
                        tags[strlen(tags) - 1] = '\0';
                    if (strcmp(tags, "0") == 0) {
                        fclose(file);
                        file = fopen(".neogit/tags/tag_name", "w");
                        fprintf(file, "%s\n", new_tag);
                    } else {
                        //weite it
                        fclose(file);
                        file = fopen(".neogit/tags/tag_name", "r");
                        FILE *file2 = fopen(".neogit/tags/tag_name2", "w");//close
                        int flag = 0;
                        while (fgets(tags, 10000, file) != NULL) {
                            if (tags[strlen(tags) - 1] == '\n')
                                tags[strlen(tags) - 1] = '\0';
                            unsigned long cn;
                            if (strlen(new_tag) >= strlen(tags)) {
                                cn = strlen(tags);
                            } else {
                                cn = strlen(new_tag);
                            }
                            for (int i = 0; i < cn; i++) {
                                if (tags[i] < new_tag[i]) {
                                    fprintf(file2, "%s\n", tags);
                                    break;
                                } else if (tags[i] > new_tag[i]) {
                                    flag = 1;
                                    fprintf(file2, "%s\n", new_tag);
                                    break;
                                }
                            }
                        }
                        if (flag == 1) {
                            fprintf(file2, "%s\n", new_tag);
                        }
                        fclose(file);
                        remove(".neogit/tags/tag_name");
                        fclose(file2);
                        rename(".neogit/tags/tag_name2", ".neogit/tags/tag_name");
                    }

                    //write in data
                    file = fopen(".neogit/tags/tag_data", "r");
                    char *tag_contant = (char *) malloc(10000 * sizeof(char));
                    fgets(tag_contant, 10000, file);
                    if (tag_contant[strlen(tag_contant) - 1] == '\n')
                        tag_contant[strlen(tag_contant) - 1] = '\0';
                    if (strcmp(tag_contant, "0") == 0) {
                        fclose(file);
                        file = fopen(".neogit/tags/tag_data", "w");
                        fprintf(file, "%s ", new_tag);
                        fprintf(file, "%s ", current_id);
                        fprintf(file, "%s ", user_name);
                        fprintf(file, "%s ", email);
                        fprintf(file, "%s ", cur_time);
                        fprintf(file, "%s\n", message);
                        return 0;
                    } else {
                        fclose(file);
                        file = fopen(".neogit/tags/tag_data", "a");
                        fprintf(file, "%s ", new_tag);
                        fprintf(file, "%s ", current_id);
                        fprintf(file, "%s ", user_name);
                        fprintf(file, "%s ", email);
                        fprintf(file, "%s ", cur_time);
                        fprintf(file, "%s\n", message);
                        return 0;
                    }
                } else if (strcmp(argv[4], "-c") == 0) {

                    //new tag
                    char *new_tag = (char *) malloc(10000 * sizeof(char));
                    strcpy(new_tag, argv[3]);

                    //check new tag
                    char *tags = (char *) malloc(10000 * sizeof(char));
                    FILE *file = fopen(".neogit/tags/tag_name", "r");
                    while (fgets(tags, 10000, file) != NULL) {
                        if (tags[strlen(tags) - 1] == '\n')
                            tags[strlen(tags) - 1] = '\0';
                        if (strcmp(new_tag, tags) == 0) {
                            printf("this tag already exists\n");
                            return 1;
                        }
                    }
                    fclose(file);

                    //id
                    char *id = (char *) malloc(10000 * sizeof(char));
                    strcpy(id, argv[5]);

                    //author
                    file = fopen(".neogit/config", "r");
                    char *user_name = (char *) malloc(10000 * sizeof(char));
                    char *email = (char *) malloc(10000 * sizeof(char));
                    fgets(user_name, 10000, file);
                    if (user_name[strlen(user_name) - 1] == '\n')
                        user_name[strlen(user_name) - 1] = '\0';
                    fgets(email, 10000, file);
                    if (email[strlen(email) - 1] == '\n')
                        email[strlen(email) - 1] = '\0';

                    //time
                    time_t cur = time(NULL);
                    char cur_time[1000] = "";
                    strcpy(cur_time, ctime(&cur));


                    //check if its first and write it
                    file = fopen(".neogit/tags/tag_name", "r");
                    fgets(tags, 10000, file);
                    if (tags[strlen(tags) - 1] == '\n')
                        tags[strlen(tags) - 1] = '\0';
                    if (strcmp(tags, "0") == 0) {
                        fclose(file);
                        file = fopen(".neogit/tags/tag_name", "w");
                        fprintf(file, "%s\n", new_tag);
                    } else {
                        //weite it
                        fclose(file);
                        file = fopen(".neogit/tags/tag_name", "r");
                        FILE *file2 = fopen(".neogit/tags/tag_name2", "w");//close
                        int flag = 0;
                        while (fgets(tags, 10000, file) != NULL) {
                            if (tags[strlen(tags) - 1] == '\n')
                                tags[strlen(tags) - 1] = '\0';
                            unsigned long cn;
                            if (strlen(new_tag) >= strlen(tags)) {
                                cn = strlen(tags);
                            } else {
                                cn = strlen(new_tag);
                            }
                            for (int i = 0; i < cn; i++) {
                                if (tags[i] < new_tag[i]) {
                                    fprintf(file2, "%s\n", tags);
                                    break;
                                } else if (tags[i] > new_tag[i]) {
                                    flag = 1;
                                    fprintf(file2, "%s\n", new_tag);
                                    break;
                                }
                            }
                        }
                        if (flag == 1) {
                            fprintf(file2, "%s\n", new_tag);
                        }
                        fclose(file);
                        remove(".neogit/tags/tag_name");
                        fclose(file2);
                        rename(".neogit/tags/tag_name2", ".neogit/tags/tag_name");
                    }

                    //write in data
                    file = fopen(".neogit/tags/tag_data", "r");
                    char *tag_contant = (char *) malloc(10000 * sizeof(char));
                    fgets(tag_contant, 10000, file);
                    if (tag_contant[strlen(tag_contant) - 1] == '\n')
                        tag_contant[strlen(tag_contant) - 1] = '\0';
                    if (strcmp(tag_contant, "0") == 0) {
                        fclose(file);
                        file = fopen(".neogit/tags/tag_data", "w");
                        fprintf(file, "%s ", new_tag);
                        fprintf(file, "%s ", id);
                        fprintf(file, "%s ", user_name);
                        fprintf(file, "%s ", email);
                        fprintf(file, "%s ", cur_time);
                        fprintf(file, "%s\n", "0");
                        return 0;
                    } else {
                        fclose(file);
                        file = fopen(".neogit/tags/tag_data", "a");
                        fprintf(file, "%s ", new_tag);
                        fprintf(file, "%s ", id);
                        fprintf(file, "%s ", user_name);
                        fprintf(file, "%s ", email);
                        fprintf(file, "%s ", cur_time);
                        fprintf(file, "%s\n", "0");
                        return 0;
                    }

                } else {
                    printf("invalid command\n");
                    return 1;
                }
            } else {
                printf("invalid command\n");
                return 1;
            }
        } else if (argc == 7) {
            if (strcmp(argv[2], "-a") == 0) {
                if ((strcmp(argv[4], "-m") == 0) && (strcmp(argv[4], "-f") == 0)) {

                    //new tag
                    char *new_tag = (char *) malloc(10000 * sizeof(char));
                    strcpy(new_tag, argv[3]);

                    //check new tag
                    int check = 0;
                    char *tags = (char *) malloc(10000 * sizeof(char));
                    FILE *file = fopen(".neogit/tags/tag_name", "r");
                    while (fgets(tags, 10000, file) != NULL) {
                        if (tags[strlen(tags) - 1] == '\n')
                            tags[strlen(tags) - 1] = '\0';
                        if (strcmp(new_tag, tags) == 0) {
                            check = 1;
                        }
                    }
                    fclose(file);

                    //message
                    char *message = (char *) malloc(10000 * sizeof(char));
                    strcpy(message, argv[5]);

                    //current id
                    file = fopen(".neogit/ccurrent_commit_id", "r");
                    char *current_id = (char *) malloc(10000 * sizeof(char));
                    fgets(current_id, 10000, file);
                    if (current_id[strlen(current_id) - 1] == '\n')
                        current_id[strlen(current_id) - 1] = '\0';
                    char *token = strtok(current_id, " ");
                    strcpy(current_id, token);
                    fclose(file);

                    //author
                    file = fopen(".neogit/config", "r");
                    char *user_name = (char *) malloc(10000 * sizeof(char));
                    char *email = (char *) malloc(10000 * sizeof(char));
                    fgets(user_name, 10000, file);
                    if (user_name[strlen(user_name) - 1] == '\n')
                        user_name[strlen(user_name) - 1] = '\0';
                    fgets(email, 10000, file);
                    if (email[strlen(email) - 1] == '\n')
                        email[strlen(email) - 1] = '\0';

                    //time
                    time_t cur = time(NULL);
                    char cur_time[1000] = "";
                    strcpy(cur_time, ctime(&cur));

                    if (check == 0) {
                        //check if its first and write it
                        file = fopen(".neogit/tags/tag_name", "r");
                        fgets(tags, 10000, file);
                        if (tags[strlen(tags) - 1] == '\n')
                            tags[strlen(tags) - 1] = '\0';
                        if (strcmp(tags, "0") == 0) {
                            fclose(file);
                            file = fopen(".neogit/tags/tag_name", "w");
                            fprintf(file, "%s\n", new_tag);
                        } else {
                            //weite it
                            fclose(file);
                            file = fopen(".neogit/tags/tag_name", "r");
                            FILE *file2 = fopen(".neogit/tags/tag_name2", "w");//close
                            int flag = 0;
                            while (fgets(tags, 10000, file) != NULL) {
                                if (tags[strlen(tags) - 1] == '\n')
                                    tags[strlen(tags) - 1] = '\0';
                                unsigned long cn;
                                if (strlen(new_tag) >= strlen(tags)) {
                                    cn = strlen(tags);
                                } else {
                                    cn = strlen(new_tag);
                                }
                                for (int i = 0; i < cn; i++) {
                                    if (tags[i] < new_tag[i]) {
                                        fprintf(file2, "%s\n", tags);
                                        break;
                                    } else if (tags[i] > new_tag[i]) {
                                        flag = 1;
                                        fprintf(file2, "%s\n", new_tag);
                                        break;
                                    }
                                }
                            }
                            if (flag == 1) {
                                fprintf(file2, "%s\n", new_tag);
                            }
                            fclose(file);
                            remove(".neogit/tags/tag_name");
                            fclose(file2);
                            rename(".neogit/tags/tag_name2", ".neogit/tags/tag_name");
                        }

                        //write in data
                        file = fopen(".neogit/tags/tag_data", "r");
                        char *tag_contant = (char *) malloc(10000 * sizeof(char));
                        fgets(tag_contant, 10000, file);
                        if (tag_contant[strlen(tag_contant) - 1] == '\n')
                            tag_contant[strlen(tag_contant) - 1] = '\0';
                        if (strcmp(tag_contant, "0") == 0) {
                            fclose(file);
                            file = fopen(".neogit/tags/tag_data", "w");
                            fprintf(file, "%s ", new_tag);
                            fprintf(file, "%s ", current_id);
                            fprintf(file, "%s ", user_name);
                            fprintf(file, "%s ", email);
                            fprintf(file, "%s ", cur_time);
                            fprintf(file, "%s\n", message);

                        } else {
                            fclose(file);
                            file = fopen(".neogit/tags/tag_data", "a");
                            fprintf(file, "%s ", new_tag);
                            fprintf(file, "%s ", current_id);
                            fprintf(file, "%s ", user_name);
                            fprintf(file, "%s ", email);
                            fprintf(file, "%s ", cur_time);
                            fprintf(file, "%s\n", message);
                        }
                        fclose(file);
                        return 0;
                    } else {
                        file = fopen(".neogit/tags/tag_data", "r");
                        char *tag_content = (char *) malloc(10000 * sizeof(char));
                        FILE *file2 = fopen(".neogit/tags/tag_data2", "w");
                        while (fgets(tag_content, 10000, file) != NULL) {
                            if (tag_content[strlen(tag_content) - 1] == '\n')
                                tag_content[strlen(tag_content) - 1] = '\0';
                            char *toke = strtok(tag_content, " ");
                            if (strcmp(toke, new_tag) == 0) {
                                fprintf(file2, "%s ", new_tag);
                                fprintf(file2, "%s ", current_id);
                                fprintf(file2, "%s ", user_name);
                                fprintf(file2, "%s ", email);
                                fprintf(file2, "%s ", cur_time);
                                fprintf(file2, "%s\n", message);
                            } else {
                                fprintf(file2, "%s\n", tag_content);
                            }
                        }
                        fclose(file);
                        remove(".neogit/tags/tag_data");
                        fclose(file2);
                        rename(".neogit/tags/tag_data2", ".neogit/tags/tag_data");
                        return 0;
                    }
                } else if ((strcmp(argv[4], "-c") == 0) && (strcmp(argv[4], "-f") == 0)) {

                    //new tag
                    char *new_tag = (char *) malloc(10000 * sizeof(char));
                    strcpy(new_tag, argv[3]);

                    //check new tag
                    int check = 0;
                    char *tags = (char *) malloc(10000 * sizeof(char));
                    FILE *file = fopen(".neogit/tags/tag_name", "r");
                    while (fgets(tags, 10000, file) != NULL) {
                        if (tags[strlen(tags) - 1] == '\n')
                            tags[strlen(tags) - 1] = '\0';
                        if (strcmp(new_tag, tags) == 0) {
                            check = 1;
                        }
                    }
                    fclose(file);

                    //id
                    char *id = (char *) malloc(10000 * sizeof(char));
                    strcpy(id, argv[5]);


                    //author
                    file = fopen(".neogit/config", "r");
                    char *user_name = (char *) malloc(10000 * sizeof(char));
                    char *email = (char *) malloc(10000 * sizeof(char));
                    fgets(user_name, 10000, file);
                    if (user_name[strlen(user_name) - 1] == '\n')
                        user_name[strlen(user_name) - 1] = '\0';
                    fgets(email, 10000, file);
                    if (email[strlen(email) - 1] == '\n')
                        email[strlen(email) - 1] = '\0';

                    //time
                    time_t cur = time(NULL);
                    char cur_time[1000] = "";
                    strcpy(cur_time, ctime(&cur));

                    if (check == 0) {
                        //check if its first and write it
                        file = fopen(".neogit/tags/tag_name", "r");
                        fgets(tags, 10000, file);
                        if (tags[strlen(tags) - 1] == '\n')
                            tags[strlen(tags) - 1] = '\0';
                        if (strcmp(tags, "0") == 0) {
                            fclose(file);
                            file = fopen(".neogit/tags/tag_name", "w");
                            fprintf(file, "%s\n", new_tag);
                        } else {
                            //weite it
                            fclose(file);
                            file = fopen(".neogit/tags/tag_name", "r");
                            FILE *file2 = fopen(".neogit/tags/tag_name2", "w");//close
                            int flag = 0;
                            while (fgets(tags, 10000, file) != NULL) {
                                if (tags[strlen(tags) - 1] == '\n')
                                    tags[strlen(tags) - 1] = '\0';
                                unsigned long cn;
                                if (strlen(new_tag) >= strlen(tags)) {
                                    cn = strlen(tags);
                                } else {
                                    cn = strlen(new_tag);
                                }
                                for (int i = 0; i < cn; i++) {
                                    if (tags[i] < new_tag[i]) {
                                        fprintf(file2, "%s\n", tags);
                                        break;
                                    } else if (tags[i] > new_tag[i]) {
                                        flag = 1;
                                        fprintf(file2, "%s\n", new_tag);
                                        break;
                                    }
                                }
                            }
                            if (flag == 1) {
                                fprintf(file2, "%s\n", new_tag);
                            }
                            fclose(file);
                            remove(".neogit/tags/tag_name");
                            fclose(file2);
                            rename(".neogit/tags/tag_name2", ".neogit/tags/tag_name");
                        }

                        //write in data
                        file = fopen(".neogit/tags/tag_data", "r");
                        char *tag_contant = (char *) malloc(10000 * sizeof(char));
                        fgets(tag_contant, 10000, file);
                        if (tag_contant[strlen(tag_contant) - 1] == '\n')
                            tag_contant[strlen(tag_contant) - 1] = '\0';
                        if (strcmp(tag_contant, "0") == 0) {
                            fclose(file);
                            file = fopen(".neogit/tags/tag_data", "w");
                            fprintf(file, "%s ", new_tag);
                            fprintf(file, "%s ", id);
                            fprintf(file, "%s ", user_name);
                            fprintf(file, "%s ", email);
                            fprintf(file, "%s ", cur_time);
                            fprintf(file, "%s\n", "0");

                        } else {
                            fclose(file);
                            file = fopen(".neogit/tags/tag_data", "a");
                            fprintf(file, "%s ", new_tag);
                            fprintf(file, "%s ", id);
                            fprintf(file, "%s ", user_name);
                            fprintf(file, "%s ", email);
                            fprintf(file, "%s ", cur_time);
                            fprintf(file, "%s\n", "0");
                        }
                        fclose(file);
                        return 0;
                    } else {
                        file = fopen(".neogit/tags/tag_data", "r");
                        char *tag_content = (char *) malloc(10000 * sizeof(char));
                        FILE *file2 = fopen(".neogit/tags/tag_data2", "w");
                        while (fgets(tag_content, 10000, file) != NULL) {
                            if (tag_content[strlen(tag_content) - 1] == '\n')
                                tag_content[strlen(tag_content) - 1] = '\0';
                            char *toke = strtok(tag_content, " ");
                            if (strcmp(toke, new_tag) == 0) {
                                fprintf(file2, "%s ", new_tag);
                                fprintf(file2, "%s ", id);
                                fprintf(file2, "%s ", user_name);
                                fprintf(file2, "%s ", email);
                                fprintf(file2, "%s ", cur_time);
                                fprintf(file2, "%s\n", "0");
                            } else {
                                fprintf(file2, "%s\n", tag_content);
                            }
                        }
                        fclose(file);
                        remove(".neogit/tags/tag_data");
                        fclose(file2);
                        rename(".neogit/tags/tag_data2", ".neogit/tags/tag_data");
                        return 0;
                    }
                } else {
                    printf("invalid command\n");
                    return 1;
                }
            } else {
                printf("invalid command\n");
                return 1;
            }
        } else if (argc == 8) {
            if ((strcmp(argv[4], "-m") == 0) && (strcmp(argv[6], "-c") == 0)) {

                //new tag
                char *new_tag = (char *) malloc(10000 * sizeof(char));
                strcpy(new_tag, argv[3]);

                //check new tag
                char *tags = (char *) malloc(10000 * sizeof(char));
                FILE *file = fopen(".neogit/tags/tag_name", "r");
                while (fgets(tags, 10000, file) != NULL) {
                    if (tags[strlen(tags) - 1] == '\n')
                        tags[strlen(tags) - 1] = '\0';
                    if (strcmp(new_tag, tags) == 0) {
                        printf("this tag already exists\n");
                        return 1;
                    }
                }
                fclose(file);

                //message
                char *message = (char *) malloc(10000 * sizeof(char));
                strcpy(message, argv[5]);

                //id
                char *id = (char *) malloc(10000 * sizeof(char));
                strcpy(id, argv[7]);

                //author
                file = fopen(".neogit/config", "r");
                char *user_name = (char *) malloc(10000 * sizeof(char));
                char *email = (char *) malloc(10000 * sizeof(char));
                fgets(user_name, 10000, file);
                if (user_name[strlen(user_name) - 1] == '\n')
                    user_name[strlen(user_name) - 1] = '\0';
                fgets(email, 10000, file);
                if (email[strlen(email) - 1] == '\n')
                    email[strlen(email) - 1] = '\0';

                //time
                time_t cur = time(NULL);
                char cur_time[1000] = "";
                strcpy(cur_time, ctime(&cur));


                //check if its first and write it
                file = fopen(".neogit/tags/tag_name", "r");
                fgets(tags, 10000, file);
                if (tags[strlen(tags) - 1] == '\n')
                    tags[strlen(tags) - 1] = '\0';
                if (strcmp(tags, "0") == 0) {
                    fclose(file);
                    file = fopen(".neogit/tags/tag_name", "w");
                    fprintf(file, "%s\n", new_tag);
                } else {
                    //weite it
                    fclose(file);
                    file = fopen(".neogit/tags/tag_name", "r");
                    FILE *file2 = fopen(".neogit/tags/tag_name2", "w");//close
                    int flag = 0;
                    while (fgets(tags, 10000, file) != NULL) {
                        if (tags[strlen(tags) - 1] == '\n')
                            tags[strlen(tags) - 1] = '\0';
                        unsigned long cn;
                        if (strlen(new_tag) >= strlen(tags)) {
                            cn = strlen(tags);
                        } else {
                            cn = strlen(new_tag);
                        }
                        for (int i = 0; i < cn; i++) {
                            if (tags[i] < new_tag[i]) {
                                fprintf(file2, "%s\n", tags);
                                break;
                            } else if (tags[i] > new_tag[i]) {
                                flag = 1;
                                fprintf(file2, "%s\n", new_tag);
                                break;
                            }
                        }
                    }
                    if (flag == 1) {
                        fprintf(file2, "%s\n", new_tag);
                    }
                    fclose(file);
                    remove(".neogit/tags/tag_name");
                    fclose(file2);
                    rename(".neogit/tags/tag_name2", ".neogit/tags/tag_name");
                }

                //write in data
                file = fopen(".neogit/tags/tag_data", "r");
                char *tag_contant = (char *) malloc(10000 * sizeof(char));
                fgets(tag_contant, 10000, file);
                if (tag_contant[strlen(tag_contant) - 1] == '\n')
                    tag_contant[strlen(tag_contant) - 1] = '\0';
                if (strcmp(tag_contant, "0") == 0) {
                    fclose(file);
                    file = fopen(".neogit/tags/tag_data", "w");
                    fprintf(file, "%s ", new_tag);
                    fprintf(file, "%s ", id);
                    fprintf(file, "%s ", user_name);
                    fprintf(file, "%s ", email);
                    fprintf(file, "%s ", cur_time);
                    fprintf(file, "%s\n", message);
                    return 0;
                } else {
                    fclose(file);
                    file = fopen(".neogit/tags/tag_data", "a");
                    fprintf(file, "%s ", new_tag);
                    fprintf(file, "%s ", id);
                    fprintf(file, "%s ", user_name);
                    fprintf(file, "%s ", email);
                    fprintf(file, "%s ", cur_time);
                    fprintf(file, "%s\n", message);
                    return 0;
                }

            } else {
                printf("invalid command\n");
                return 1;
            }
        }  else if (argc == 9) {
            if ((strcmp(argv[4], "-m") == 0) && (strcmp(argv[6], "-c") == 0) && (strcmp(argv[8], "-f") == 0)) {
                //new tag
                char *new_tag = (char *) malloc(10000 * sizeof(char));
                strcpy(new_tag, argv[3]);

                //check new tag
                int check = 0;
                char *tags = (char *) malloc(10000 * sizeof(char));
                FILE *file = fopen(".neogit/tags/tag_name", "r");
                while (fgets(tags, 10000, file) != NULL) {
                    if (tags[strlen(tags) - 1] == '\n')
                        tags[strlen(tags) - 1] = '\0';
                    if (strcmp(new_tag, tags) == 0) {
                        check = 1;
                    }
                }
                fclose(file);

                //message
                char *message = (char *) malloc(10000 * sizeof(char));
                strcpy(message, argv[5]);

                //id
                char *id = (char *) malloc(10000 * sizeof(char));
                strcpy(id, argv[7]);


                //author
                file = fopen(".neogit/config", "r");
                char *user_name = (char *) malloc(10000 * sizeof(char));
                char *email = (char *) malloc(10000 * sizeof(char));
                fgets(user_name, 10000, file);
                if (user_name[strlen(user_name) - 1] == '\n')
                    user_name[strlen(user_name) - 1] = '\0';
                fgets(email, 10000, file);
                if (email[strlen(email) - 1] == '\n')
                    email[strlen(email) - 1] = '\0';

                //time
                time_t cur = time(NULL);
                char cur_time[1000] = "";
                strcpy(cur_time, ctime(&cur));

                if (check == 0) {
                    //check if its first and write it
                    file = fopen(".neogit/tags/tag_name", "r");
                    fgets(tags, 10000, file);
                    if (tags[strlen(tags) - 1] == '\n')
                        tags[strlen(tags) - 1] = '\0';
                    if (strcmp(tags, "0") == 0) {
                        fclose(file);
                        file = fopen(".neogit/tags/tag_name", "w");
                        fprintf(file, "%s\n", new_tag);
                    } else {
                        //weite it
                        fclose(file);
                        file = fopen(".neogit/tags/tag_name", "r");
                        FILE *file2 = fopen(".neogit/tags/tag_name2", "w");//close
                        int flag = 0;
                        while (fgets(tags, 10000, file) != NULL) {
                            if (tags[strlen(tags) - 1] == '\n')
                                tags[strlen(tags) - 1] = '\0';
                            unsigned long cn;
                            if (strlen(new_tag) >= strlen(tags)) {
                                cn = strlen(tags);
                            } else {
                                cn = strlen(new_tag);
                            }
                            for (int i = 0; i < cn; i++) {
                                if (tags[i] < new_tag[i]) {
                                    fprintf(file2, "%s\n", tags);
                                    break;
                                } else if (tags[i] > new_tag[i]) {
                                    flag = 1;
                                    fprintf(file2, "%s\n", new_tag);
                                    break;
                                }
                            }
                        }
                        if (flag == 1) {
                            fprintf(file2, "%s\n", new_tag);
                        }
                        fclose(file);
                        remove(".neogit/tags/tag_name");
                        fclose(file2);
                        rename(".neogit/tags/tag_name2", ".neogit/tags/tag_name");
                    }

                    //write in data
                    file = fopen(".neogit/tags/tag_data", "r");
                    char *tag_contant = (char *) malloc(10000 * sizeof(char));
                    fgets(tag_contant, 10000, file);
                    if (tag_contant[strlen(tag_contant) - 1] == '\n')
                        tag_contant[strlen(tag_contant) - 1] = '\0';
                    if (strcmp(tag_contant, "0") == 0) {
                        fclose(file);
                        file = fopen(".neogit/tags/tag_data", "w");
                        fprintf(file, "%s ", new_tag);
                        fprintf(file, "%s ", id);
                        fprintf(file, "%s ", user_name);
                        fprintf(file, "%s ", email);
                        fprintf(file, "%s ", cur_time);
                        fprintf(file, "%s\n", message);

                    } else {
                        fclose(file);
                        file = fopen(".neogit/tags/tag_data", "a");
                        fprintf(file, "%s ", new_tag);
                        fprintf(file, "%s ", id);
                        fprintf(file, "%s ", user_name);
                        fprintf(file, "%s ", email);
                        fprintf(file, "%s ", cur_time);
                        fprintf(file, "%s\n", message);
                    }
                    fclose(file);
                    return 0;
                } else {
                    file = fopen(".neogit/tags/tag_data", "r");
                    char *tag_content = (char *) malloc(10000 * sizeof(char));
                    FILE *file2 = fopen(".neogit/tags/tag_data2", "w");
                    while (fgets(tag_content, 10000, file) != NULL) {
                        if (tag_content[strlen(tag_content) - 1] == '\n')
                            tag_content[strlen(tag_content) - 1] = '\0';
                        char *toke = strtok(tag_content, " ");
                        if (strcmp(toke, new_tag) == 0) {
                            fprintf(file2, "%s ", new_tag);
                            fprintf(file2, "%s ", id);
                            fprintf(file2, "%s ", user_name);
                            fprintf(file2, "%s ", email);
                            fprintf(file2, "%s ", cur_time);
                            fprintf(file2, "%s\n", message);
                        } else {
                            fprintf(file2, "%s\n", tag_content);
                        }
                    }
                    fclose(file);
                    remove(".neogit/tags/tag_data");
                    fclose(file2);
                    rename(".neogit/tags/tag_data2", ".neogit/tags/tag_data");
                    return 0;
                }
            } else {
                printf("invalid command\n");
                return 1;
            }

            }

        }
    }
    else if (argc == 2) {
    } else {
        printf("invalid command\n");
        return 1;
    }
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]) {

    if (argc < 2) {
        fprintf(stdout, "please enter a valid command\n");
        return 1;
    }//printf("1\n");
    if (strcmp(argv[1], "init") == 0) {
        return run_init(argc, argv);
    }


    if (strcmp(true_command(argv[1]), "init") == 0) {
        return run_init(argc, argv);
    }
    if ((strcmp(true_command(argv[1]), "config") == 0) || (strcmp(argv[1], "config") == 0)) {
        return run_config(argc, argv);
        printf("1\n");
    } else if ((strcmp(argv[1], "add") == 0) || (strcmp(true_command(argv[1]), "add") == 0)) {
        return run_add(argc, argv);
        printf("1\n");
    } else if ((strcmp(argv[1], "reset") == 0) || (strcmp(true_command(argv[1]), "reset") == 0)) {
        return run_reset(argc, argv);
        printf("1\n");
    } else if ((strcmp(argv[1], "commit") == 0) || (strcmp(true_command(argv[1]), "commit") == 0)) {
        return run_commit(argc, argv);
        printf("1\n");
    } else if ((strcmp(argv[1], "log") == 0) || (strcmp(true_command(argv[1]), "log") == 0)) {
        return run_log(argc, argv);
    } else if ((strcmp(argv[1], "branch") == 0) || (strcmp(true_command(argv[1]), "branch") == 0)) {
        return run_branch(argc, argv);
    } else if ((strcmp(argv[1], "checkout") == 0) || (strcmp(true_command(argv[1]), "checkout") == 0)) {
        return run_checkout(argc, argv);
    } else if ((strcmp(argv[1], "status") == 0) || (strcmp(true_command(argv[1]), "status") == 0)) {
        return run_status(argc, argv);
    } else if ((strcmp(argv[1], "set") == 0) || (strcmp(true_command(argv[1]), "set") == 0)) {
        return run_set(argc, argv);
    } else if ((strcmp(argv[1], "replace") == 0) || (strcmp(true_command(argv[1]), "replace") == 0)) {
        return run_replace(argc, argv);
    } else if ((strcmp(argv[1], "remove") == 0) || (strcmp(true_command(argv[1]), "remove") == 0)) {
        return run_remove(argc, argv);
    } else if ((strcmp(argv[1], "diff") == 0) || (strcmp(true_command(argv[1]), "diff") == 0)) {
        return run_diff(argc, argv);
    } else if ((strcmp(argv[1], "tag") == 0) || (strcmp(true_command(argv[1]), "tag") == 0)) {
        return run_tag(argc, argv);
    } else {
        printf("invalid command\n");
    }

    return 0;
}