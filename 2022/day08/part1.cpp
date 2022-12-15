#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>

class Tree
{
public:
        int m_height;
        bool m_visible;

        Tree(int height, bool visible)
                : m_height{height}, m_visible{visible}
        {
        }
};

class Grid
{
public:
        std::vector<Tree> m_entries;
        int m_height;
        int m_length;

        void
        add_sizes(int height, int length)
        {
                m_height = height;
                m_length = length;
        }

        Tree&
        get(int x, int y)
        {
                return m_entries[y * m_length + x]; 
        }

        void
        add(Tree value)
        {
                m_entries.push_back(value);
        }

        int
        count_visible(void)
        {
                int count = 0;
                for (int x = 0; x < m_length; ++x) 
                        for (int y = 0; y < m_height; ++y) {
                                Tree &tree = get(x, y);
                                count += tree.m_visible;
                        }

                return count;
        }
};

void
parse(std::fstream &input, Grid *&grid)
{
        grid = new Grid();

        std::string line;
        int length, height = 0;
        while (std::getline(input, line)) {

                static int c;
                if (!c++) 
                        length = line.size();
                        
                ++height;
                for (auto c : line) 
                        grid->add(Tree(c - '0', 0));
        }

        grid->add_sizes(height, length);
}

void
check_line(Grid *grid, int line, bool vertical)
{
        int min = -1;
        int limit = (vertical) ? grid->m_height : grid->m_length;
        for (int i = 0; i < limit; ++i) {

                Tree &tree = (vertical) ? grid->get(line, i) : grid->get(i, line);
                if (tree.m_height > min) {
                        tree.m_visible = true;
                        min = tree.m_height;
                }
        }

        min = -1;
        int i = (vertical) ? (grid->m_height - 1) : (grid->m_length - 1);
        for (; i >= 0; --i) {

                Tree &tree = (vertical) ? grid->get(line, i) : grid->get(i, line);
                if (tree.m_height > min) {
                        tree.m_visible = true;
                        min = tree.m_height;
                }
        }
}

int
solve(std::fstream &input)
{
        Grid *grid;
        parse(input, grid);

        for (int i = 0; i < grid->m_length; ++i)
                check_line(grid, i, true);

        for (int i = 0; i < grid->m_height; ++i)
                check_line(grid, i, false);

        return grid->count_visible();
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
