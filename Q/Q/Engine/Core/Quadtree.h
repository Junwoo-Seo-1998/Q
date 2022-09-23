/*
    File Name: Quadtree.h
    Project Name: Q
    Author(s):
        Primary: Hoseob Jeong
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <functional>
#include "Rect.h"


namespace q_engine
{

	
    enum QuadrantPart
    {
          NW = 0
        , NE
        , SW
    	, SE
    };

	
    template<typename T, typename GetRect>
    class Quadtree
    {
    private:
        struct Node
        {
            std::array<std::unique_ptr<Node>, 4> children;
            std::vector<T> values;
        };
    public:
     
        Quadtree(const Rect& rect = { {0.f,0.f},{0.f,0.f} }, const GetRect& getRect = GetRect()) : mRect(rect), mRoot(std::make_unique<Node>()), mGetRect(getRect)
        {
        }
        Quadtree& operator=(const Quadtree& a) { *this = a; };
        Quadtree(const GetRect& getrect) : mGetRect(getrect) {}
    	Rect ComputeBox(const Rect& rect, int i) const;
        int GetQuadrant(const Rect& nodeRect, const Rect& valueRect) const;
        void SetInitRect(Rect rect);
        void Add(const T& collisionID);
        void Add_helper(Node* node, std::size_t depth, const Rect& rect, const T& valueID);
        void Merge(Node* node);
        void split(Node* node, const Rect& rect);
        void Remove(const T& collisionID);
        void Remove_helper(Node* node, Node* parent, const Rect& rect, const T& valueID);
        void remove_value(Node* node, const T& valueID);
        void tryingMerge(Node* node);
        std::vector<T> query(const Rect& rect) const;
        void query(Node* node, const Rect& rect, const Rect& queryRect, std::vector<T>& valueIDs) const;
        std::vector<std::pair<T, T>> FindAllIntersection() const;
        void FindAllIntersection_helper(Node* node, std::vector<std::pair<T, T>>& intersections) const;
        void FindIntersectionDownward(Node* node, const T& valueID, std::vector<std::pair<T, T>>& intersections) const;
        void SetGameState(GameState* state);
    private:
       

        //maximum number of objects need to split it
        static constexpr auto ThresHold = std::size_t(128);
        // height of quadtree
        static constexpr auto MaxDepth = std::size_t(8);

        Rect mRect;
        std::unique_ptr<Node> mRoot;
        GetRect mGetRect;
        
        GameState* mGamestate = nullptr;
    	
        bool isLeaf(const Node* node) const
        {
            return !static_cast<bool>(node->children[0]);
        }
    };
}

#include "Quadtree.inl"
