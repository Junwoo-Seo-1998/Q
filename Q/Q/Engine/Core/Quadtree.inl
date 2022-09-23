/*
    File Name: Quadtree.inl
    Project Name: Q
    Author(s):
        Primary: Hoseob Jeong
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include "Quadtree.h"
#include <stdexcept>

namespace q_engine
{
	template <typename T, typename GetRect>
    Rect Quadtree<T, GetRect>::ComputeBox(const Rect& rect, int i) const
    {
		
        glm::vec2 origin_bottomleft = rect.bottomLeft;
		glm::vec2 child_size = rect.Size() / 2.f;

		switch(i)
		{
        case QuadrantPart::NW:
            return Rect{ glm::vec2{origin_bottomleft.x , origin_bottomleft.y + child_size.y}, child_size, true };
        case QuadrantPart::NE:
            return Rect{ glm::vec2{origin_bottomleft.x + child_size.x,origin_bottomleft.y+child_size.y}, child_size, true };
        case QuadrantPart::SW:
            return Rect{ origin_bottomleft, child_size, true };
        case QuadrantPart::SE:
            return Rect{ glm::vec2{origin_bottomleft.x + child_size.x,origin_bottomleft.y},child_size, true };
        default:
            throw std::runtime_error("not Invaild the index about quadrant!");
            
		}

    }

	template <typename T, typename GetRect>
	int Quadtree<T, GetRect>::GetQuadrant(const Rect& nodeRect, const Rect& valueRect) const
	{
        glm::vec2 center = nodeRect.Center();
	
        bool startIsNorth = valueRect.Bottom() > center.y;
        bool startIsWest = valueRect.Left() < center.x;
        bool endIsEast = valueRect.Right() > center.x;
        bool endIsSouth = valueRect.Top() < center.y;

        if (startIsNorth && endIsEast) {
            return QuadrantPart::NE;
        }

        //top-left quad
        if (startIsWest && startIsNorth) {
            return QuadrantPart::NW;
        }

        //bottom-left quad
        if (startIsWest && endIsSouth) {
            return QuadrantPart::SW;
        }

        //bottom-right quad
        if (endIsEast && endIsSouth) {
            return QuadrantPart::SE;
        }
        return -1;
	}

	template <typename T, typename GetRect>
	void Quadtree<T, GetRect>::SetInitRect(Rect rect)
	{
        mRect = rect;
	}

	template<typename T, typename GetRect>
    void Quadtree<T, GetRect>::Add(const T& collisionID)
    {
        Add_helper(mRoot.get(), 0, mRect, collisionID);
    }

    template<typename T, typename GetRect>
    void Quadtree<T, GetRect>::Add_helper(Node* node, std::size_t depth, const Rect& rect, const T& valueID)
    {
        assert(node != nullptr);
		if(isLeaf(node))
		{
			if(depth >= MaxDepth || node->values.size() < ThresHold)
			{
                node->values.push_back(valueID);
			}
            else
            {
                split(node, rect);
                Add_helper(node, depth, rect, valueID);
            }
		}
        else
        {
            int quadrantposition = GetQuadrant(rect, mGetRect(valueID,mGamestate));
        	if(quadrantposition != -1)
        	{
                Add_helper(node->children[static_cast<std::size_t>(quadrantposition)].get(), depth + 1, ComputeBox(rect, quadrantposition), valueID);
        	}
            else
            {
                node->values.push_back(valueID);
            }
        }
    }

    template<typename T, typename GetRect>
     void Quadtree<T, GetRect>::split(Node* node, const Rect& rect)
    {
         assert(node != nullptr);
         assert(isLeaf(node) && "Only leaves can be split");
         for(auto& child : node->children)
         {
             child = std::make_unique<Node>();
         }
         auto new_values = std::vector<T>();
         for(const auto & value : node->values)
         {
             int quadrantposition = GetQuadrant(rect, mGetRect(value, mGamestate));
         	if(quadrantposition != -1)
         	{
                node->children[static_cast<std::size_t>(quadrantposition)]->values.push_back(value);
         	}
            else
            {
                new_values.push_back(value);
            }
         }

             node->values = std::move(new_values);
    }

	template <typename T, typename GetRect>
	void Quadtree<T, GetRect>::Remove(const T& collisionID)
	{
        Remove_helper(mRoot.get(), nullptr, mRect, collisionID);
	}

	template <typename T, typename GetRect>
	void Quadtree<T, GetRect>::Remove_helper(Node* node, Node* parent, const Rect& rect, const T& valueID)
	{
		if(isLeaf(node))
		{
            remove_value(node, valueID);

			//merge the parent
			if(parent != nullptr)
			{
                tryingMerge(parent);
			}
			
		}
        else
        {
            int quadrantposition = GetQuadrant(rect, mGetRect(valueID, mGamestate));
        	if(quadrantposition != -1)
        	{
        		// remove the valueID in a child  if value is entirely contained in it
                Remove_helper(node->children[static_cast<std::size_t>(quadrantposition)].get(), node, ComputeBox(rect, quadrantposition), valueID);
        	}
            else
            {
                remove_value(node, valueID);
            }
        }
	}

	template <typename T, typename GetRect>
	void Quadtree<T, GetRect>::remove_value(Node* node, const T& valueID)
	{
        auto iter = std::find_if(std::begin(node->values), std::end(node->values), [this, &valueID](const auto &rhs)
        {
                if (valueID == rhs) { return true; }
            else { return false; }
        });
		if(iter == std::end(node->values))
		{
            throw std::runtime_error("Trying to remove is not in present in the node!!!!");
		}
		// swap the end of element
        std::swap(*iter, node->values.back());
        //*iter = std::move(node->values.back());
		
		//remove last elements
        node->values.pop_back();
	}

	template <typename T, typename GetRect>
	void Quadtree<T, GetRect>::tryingMerge(Node* node)
	{
        auto node_valuesize = node->values.size();
		for(const auto& child : node->children)
		{
            if (!isLeaf(child.get()))
                return;
            node_valuesize += child->values.size();
		}
		if(node_valuesize <= ThresHold)
		{
            node->values.reserve(node_valuesize);
			for(const auto& child : node->children)
			{
				for(const auto& value : child->values)
				{
                    node->values.push_back(value);
				}
			}
			
			for (auto& child : node->children)
			{
                child.reset();
			}
		}
		
	}

	template <typename T, typename GetRect>
	std::vector<T> Quadtree<T, GetRect>::query(const Rect& rect) const
	{
        auto values = std::vector<T>();
        query(mRoot.get(), mRect, rect, values);
        return values;
	}

	template <typename T, typename GetRect>
	void Quadtree<T, GetRect>::query(Node* node, const Rect& rect, const Rect& queryRect,std::vector<T>& valueIDs) const
	{
        assert(node != nullptr);
        //assert(queryRect.intersects(rect));
        for(const auto& value : node->values)
        {
            if (queryRect.intersects(mGetRect(value, mGamestate)))
            {
                valueIDs.push_back(value);
            }
        }
		if(!isLeaf(node))
		{
			for(std::size_t i = 0; i < node->children.size(); i++)
			{
                auto childRect = ComputeBox(rect, static_cast<int>(i));
				if(queryRect.intersects(childRect))
				{
                    query(node->children[i].get(), childRect, queryRect, valueIDs);
				}
			}
		}
	}

	
	template <typename T, typename GetRect>
	std::vector<std::pair<T, T>> Quadtree<T, GetRect>::FindAllIntersection() const
	{
        std::vector<std::pair<T, T>> intersectdata = std::vector<std::pair<T, T>>();
        FindAllIntersection_helper(mRoot.get(), intersectdata);
        return intersectdata;
	}

	
	template <typename T, typename GetRect>
	void Quadtree<T, GetRect>::FindAllIntersection_helper(Node* node,std::vector<std::pair<T, T>>& intersections) const
	{
        for(std::size_t index1 = 0; index1 < node->values.size(); ++index1)
        {
	        for(std::size_t index2 = 0; index2 < index1; ++index2)
	        {
		        if(mGetRect(node->values[index1]).intersects(mGetRect(node->values[index2], mGamestate)))
		        {
                    intersections.emplace_back(node->values[index1], node->values[index2]);
		        }
	        }
        }

		if(!isLeaf(node))
		{
            for(const auto& child : node->children)
            {
				for(const auto& value : node->values)
				{
                    FindIntersectionDownward(child.get(), value, intersections);
				}
            }
			for(const auto& child : node->children)
			{
                FindAllIntersection_helper(child.get(), intersections);
			}
			
		}

		
	}

	template <typename T, typename GetRect>
	void Quadtree<T, GetRect>::FindIntersectionDownward(Node* node, const T& valueID,std::vector<std::pair<T, T>>& intersections) const
	{
        for(const auto& otherID : node->values)
        {
            if(mGetRect(valueID, mGamestate).intersects(mGetRect(otherID , mGamestate)))
            {
                intersections.emplace_back(valueID, otherID);
            }
        	
        }
		if(!isLeaf(node))
		{
			for(const auto& child : node->children)
			{
                FindIntersectionDownward(child.get(), valueID, intersections);
			}
		}
	}

	template <typename T, typename GetRect>
	void Quadtree<T, GetRect>::SetGameState(GameState* state) 
	{
        mGamestate = state;
	}
}
