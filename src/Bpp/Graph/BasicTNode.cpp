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
    sons_[i]->removeFather(this);
  }
}

BasicTNode::BasicTNode(const BasicTNode& node):
  sons_(node.sons_),
  father_(node.father_)
{};

