#include <iostream>
#include <cstring>
#include <fstream>
#include <memory>
#include <map>
#include <vector>
#include <limits>

using llong = long long;
constexpr llong MAX_DSPACE = 40000000;

enum Inode_type {
        T_FILE,
        T_DIR,
};

class Inode
{
public:
        llong m_size;
        int m_type;
        std::string m_name;
        std::map<std::string, Inode*> m_entries;
        Inode* m_parent;

        Inode(std::string name, int type, llong size = 0, Inode* parent = nullptr)
                : m_name{name}, m_type{type}, m_size{size}, m_entries{}, m_parent{parent}
        {
        }

        void
        add_entry(Inode *child)
        {
                m_entries[child->m_name] = child;
                child->m_parent = this;

                Inode *inode = this;
                for (; inode; inode = inode->m_parent)
                        inode->m_size += child->m_size;
        }
};

Inode*
change_dir(Inode* dir, std::string& line)
{
        std::string dir_name = line.substr(5);

        if (dir_name[0] == '.' && dir_name[1] == '.')
                return dir->m_parent;

        Inode *new_entry = new Inode(dir_name, T_DIR);
        dir->add_entry(new_entry);
        return new_entry;
}

void
list_dir(Inode* dir, std::fstream& input)
{
        std::string line;
        llong pos = input.tellp();
        while (std::getline(input, line)) {
                if (line[0] == '$') {
                        input.seekg(pos);
                        break;
                }
                
                int space = line.find(' ');
                std::string size_str = line.substr(0, space);
                std::string name = line.substr(space + 1);

                llong size = (size_str[0] == 'd') ? 0 : std::stoi(size_str);
                int type = (size_str[0] == 'd') ? T_DIR : T_FILE;
                
                Inode *new_entry = new Inode(name, type, size);
                dir->add_entry(new_entry);
                
                pos = input.tellp();
        }
}

Inode*
parse(std::fstream& input)
{
        std::string line;
        std::getline(input, line);
        std::string dir_name = line.substr(5);
        Inode *root = new Inode(dir_name, T_DIR);

        Inode *act_dir = root;
        while (std::getline(input, line)) {
                if (line[0] != '$')
                        continue;

                switch (line[2]) {
                case 'c':
                        act_dir = change_dir(act_dir, line);
                        break;
                case 'l':
                        list_dir(act_dir, input);
                        break;
                }
        }

        return root;
}

llong
sum_dir(Inode *dir, llong dspace_delete, llong actual_min)
{
        llong min = actual_min;
        for (auto const& entry : dir->m_entries) {
                if (entry.second->m_type != T_DIR)
                        continue;

                if (entry.second->m_size > dspace_delete && entry.second->m_size < min)
                        min = entry.second->m_size;
                
                min = sum_dir(entry.second, dspace_delete, min);
        }

        return min;
}

int
solve(std::fstream& input)
{
        llong total_size = 0;

        Inode *root = parse(input);
        llong dspace_delete = root->m_size - MAX_DSPACE;
        
        total_size = sum_dir(root, dspace_delete, std::numeric_limits<llong>::max());
        
        return total_size;
}

int
main(int argc, char *argv[])
{
        if (argc < 2) {
                std::cout << "[ERROR] missing argument, should be [input]\n";
                return 1;
        }

        std::fstream input{argv[1]};
        if (input.fail())
                std::cout << "[ERROR] " << strerror(errno) << '\n';

        std::cout << solve(input) << '\n';
}
