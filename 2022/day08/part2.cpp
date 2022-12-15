#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>

class Tree
{
public:
        int m_height;
        int m_score;

        Tree(int height, int score)
                : m_height{height}, m_score{score}
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
        count_max(void)
        {
                int max = 0;
                for (int x = 0; x < m_length; ++x) 
                        for (int y = 0; y < m_height; ++y)
                                if (get(x, y).m_score > max)
                                        max = get(x, y).m_score;

                return max;
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
                        grid->add(Tree(c - '0', 1));
        }

        grid->add_sizes(height, length);
}

void
check_line(Grid *grid, int x, int y, bool vertical)
{
        Tree &tree = grid->get(x, y);

        int mult = 0;
        int limit = (vertical) ? grid->m_height : grid->m_length;
        for (int i = ((vertical) ? y : x) + 1; i < limit; ++i, ++mult) {
                
                int tmp_height = (vertical) ? grid->get(x, i).m_height : grid->get(i, y).m_height;
                if (tree.m_height <= tmp_height) {
                        ++mult;
                        break;
                }
        }
        tree.m_score *= mult;
        
        mult = 0;
        for (int i = ((vertical) ? y : x) - 1; i >= 0; --i, ++mult) {

                int tmp_height = (vertical) ? grid->get(x, i).m_height : grid->get(i, y).m_height;
                if (tree.m_height <= tmp_height) {
                        ++mult;
                        break;
                }
        }
        tree.m_score *= mult;
}

void
check_tree(Grid *grid, int x, int y)
{
        check_line(grid, x, y, true);
        check_line(grid, x, y, false);
}

int
solve(std::fstream &input)
{
        Grid *grid;
        parse(input, grid);

        for (int y = 0; y < grid->m_length; ++y)
                for (int x = 0; x < grid->m_height; ++x)
                        check_tree(grid, x, y);

        return grid->count_max();
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
