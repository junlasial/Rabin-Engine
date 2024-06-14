struct Node {
    int row, col;
    float gCost, hCost, fCost;
    Node* parent;

    Node() : row(0), col(0), gCost(0), hCost(0), fCost(0), parent(nullptr) {}
    Node(int r, int c) : row(r), col(c), gCost(0), hCost(0), fCost(0), parent(nullptr) {}

    void calculateFCost() {
        fCost = gCost + hCost;
    }

    bool operator<(const Node& other) const {
        return fCost < other.fCost;
    }
};
