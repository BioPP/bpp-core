// 
// File:    BasicTNode.cpp
// Author:  Sylvain Gaillard
// Created: 14/01/2011 14:59:07
// 

#include "BasicTNode.h"

using namespace bpp;

BasicTNode::~BasicTNode() {
  if (father_) {
    father_->removeSon(this);
  }
  for (size_t i = 0 ; i < sons_.size() ; i++) {
    sons_[i]->removeFather();
  }
}

BasicTNode::BasicTNode(const BasicTNode& node):
  sons_(node.sons_),
  father_(node.father_)
{};

BasicTNode& BasicTNode::operator=(const BasicTNode& node) {
  sons_ = node.sons_;
  father_ = node.father_;
  return * this;
}

// Neighbors

const BasicTNode* BasicTNode::getNeighbor(int pos) const {
  if (pos < 0 || pos > static_cast<int>(sons_.size())) {
    throw IndexOutOfBoundsException("BasicTNode::getNeighbor() pos is out of bounds", pos, 0, static_cast<int>(sons_.size()));
  }
  if (pos == 0)
    return father_;
  else
    return sons_[static_cast<size_t>(pos)];
}

BasicTNode* BasicTNode::getNeighbor(int pos) {
  if (pos < 0 || pos > static_cast<int>(sons_.size())) {
    throw IndexOutOfBoundsException("BasicTNode::getNeighbor() pos is out of bounds", pos, 0, static_cast<int>(sons_.size()));
  }
  if (pos == 0)
    return father_;
  else
    return sons_[static_cast<size_t>(pos)];
}

const BasicTNode* BasicTNode::operator[](int i) const {
  if (i < 0) {
    return father_;
  } else {
    return sons_[static_cast<size_t>(i)];
  }
}

BasicTNode* BasicTNode::operator[](int i) {
  if (i < 0) {
    return father_;
  } else {
    return sons_[static_cast<size_t>(i)];
  }
}

// Fathers

const BasicTNode* BasicTNode::getFather(int pos) const {
  if (pos != 0) {
    throw IndexOutOfBoundsException("BasicTNode::getFather() pos must be 0 for TNode", pos, 0, 0);
  }
  return getFather();
}

BasicTNode* BasicTNode::getFather(int pos) {
  if (pos != 0) {
    throw IndexOutOfBoundsException("BasicTNode::getFather() pos must be 0 for TNode", pos, 0, 0);
  }
  return getFather();
}

const BasicTNode* BasicTNode::getFather() const {
  return father_;
}

BasicTNode* BasicTNode::getFather() {
  return father_;
}

bool BasicTNode::isFather(const BasicTNode* node) const {
  if (father_ == node)
    return true;
  return false;
}

void BasicTNode::addFather(BasicTNode* node) {
  if (!node)
    throw NullPointerException("BasicTNode::addFather() Empty node given as input");
  if (!isFather(node))
    father_ = node;
  if (!node->isSon(this))
    node->addSon(this);
}

BasicTNode* BasicTNode::removeFather() {
  if (hasFathers()) {
    BasicTNode* father = father_;
    father_ = 0;
    father->removeSon(this);
    return father;
  }
  return 0;
}

// Sons

const BasicTNode* BasicTNode::getSon(int pos) const {
  if (pos < 0 || pos > static_cast<int>(sons_.size()) - 1) {
    throw IndexOutOfBoundsException("BasicTNode::getSon() pos out of range", pos, 0, sons_.size() - 1);
  }
  return sons_[static_cast<size_t>(pos)];
}

BasicTNode* BasicTNode::getSon(int pos) {
  if (pos < 0 || pos > static_cast<int>(sons_.size()) - 1) {
    throw IndexOutOfBoundsException("BasicTNode::getSon() pos out of range", pos, 0, sons_.size() - 1);
  }
  return sons_[static_cast<size_t>(pos)];
}

bool BasicTNode::isSon(const BasicTNode* node) const {
  for (size_t i = 0 ; i < sons_.size() ; i++) {
    if (sons_[i] == node)
      return true;
  }
  return false;
}

void BasicTNode::addSon(BasicTNode* node) {
  if (!node)
    throw NullPointerException("BasicTNode::addSon() Empty node given as input.");
  if (!isSon(node))
    sons_.push_back(node);
  if (!node->isFather(this))
    node->addFather(this);
}

BasicTNode* BasicTNode::removeSon(BasicTNode* node) {
  if (!node)
    throw NullPointerException("BasicTNode::removeSon() Empty node given as input.");
  for (size_t i = 0 ; i < sons_.size() ; i++) {
    if (sons_[i] == node) {
      sons_.erase(sons_.begin() + i);
      node->removeFather();
      return node;
    }
  }
  return 0; // return 0 if son is not found
}
