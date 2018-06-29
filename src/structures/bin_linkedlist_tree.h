#pragma once

/** <copyright github.com/jrezzende> */

#ifndef INCLUDED_BIN_LINKED_LIST_TREE
#define INCLUDED_BIN_LINKED_LIST_TREE

#include <cstdint>
#include <stdexcept>
#include <fstream>
#include <cstdio>

#include "linked_list.h"

namespace structures {

   /**CLASS BINLINKEDLIST TREE*/

   class BinLinkedListTree {

      class TreeNode {
       public:
          char key_[60]{"-"};
          size_t left_{ 0u }, right_{ 0u }, list_head{ 0u };

          ~TreeNode()= default;
          TreeNode()= default;

          explicit TreeNode(const char* key) { strcpy(key_, key); }
          TreeNode(const char* key, const size_t list_head_) { strcpy(key_, key); list_head= list_head_; }
   };

      class LinkedListNode {
      public:
         size_t manpage { 0u }, nextPage { 0u };

         ~LinkedListNode()= default;
         LinkedListNode()= default;

         explicit LinkedListNode(const size_t page) : manpage(page) {}
         LinkedListNode(const size_t page, const size_t nextPage_) : manpage(page), nextPage(nextPage_) {}
      };

   public:
      size_t depth_{ 0u }, size_{ 0u };

      ~BinLinkedListTree()= default;
      BinLinkedListTree();
      
      void insert(const char* key, const size_t page);
      bool empty() const;
      size_t size() const;
      size_t depth() const;
      size_t fileSize() const;

      LinkedList<size_t>* search(const char* required) const;
      LinkedList<size_t>* disjunctive_search(const char* firstWord, const char* secondWord) const;
      LinkedList<size_t>* conjunctive_search(const char* firstWord, const char* secondWord) const;
   };

   BinLinkedListTree::BinLinkedListTree() {
      std::fstream tree("./second_tree.dat", std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
      tree.close();
   }

   void BinLinkedListTree::insert(const char* key, const size_t manpage) {

      std::fstream tree("./second_tree.dat", std::ios::in | std::ios::out | std::ios::binary);

      char node_key[60];
      int compare = 1;

      size_t aux = 0, offset = 0u, next = 0u, current = 0u, level = 0u,
      offset_left = sizeof(TreeNode::key_) + 4,
      offset_right = offset_left + sizeof(size_t),
      offset_list_head = offset_right + sizeof(size_t);

      tree.seekg(0);

      while (tree.good() && size_ != 0) {
         offset = tree.tellg();

         tree.read(node_key, sizeof(TreeNode::key_));
         compare = strcmp(key, node_key);

         if (compare != 0) {
            current = compare < 0 ? offset + offset_left : offset + offset_right;

            tree.seekg(current);
            tree.read(reinterpret_cast<char*>(&next), sizeof(size_t));

            if (next == 0u)
               break;

            tree.seekg(next);

         } else {

            tree.seekg(offset + offset_list_head);
            tree.read(reinterpret_cast<char*>(&next), sizeof(size_t));

            tree.seekp(0, std::ios::end);
            aux = tree.tellp();

            tree.seekp(offset + offset_list_head);
            tree.write(reinterpret_cast<char*>(&aux), sizeof(size_t));

            LinkedListNode *lnode = new LinkedListNode(manpage, next);
            tree.seekp(aux);
            tree.write(reinterpret_cast<char*>(lnode), sizeof(LinkedListNode));
            delete lnode;
            break;
         }
         ++level;
      }
      ++level;

      if (next == 0) {
         tree.seekp(0, std::ios::end);
         next = tree.tellp();

         TreeNode *tnode = new TreeNode(key, next + sizeof(TreeNode));
         LinkedListNode* lnode = new LinkedListNode(manpage);
         if (tnode == nullptr || lnode == nullptr)
            throw std::out_of_range("TREE IS FULL!");

         if (size_ != 0) {
            tree.seekp(current);
            tree.write(reinterpret_cast<char*>(&next), sizeof(size_t));
         }

         tree.seekp(next);
         tree.write(reinterpret_cast<char*>(tnode), sizeof(TreeNode));
         tree.write(reinterpret_cast<char*>(lnode), sizeof(LinkedListNode));
         ++size_;
         delete tnode;
         delete lnode;
      }

      depth_ = level > depth_ ? level : depth_;
      tree.close();
   }

   LinkedList<size_t>* BinLinkedListTree::search(const char* wanted) const {
      std::fstream tree("./second_tree.dat", std::ios::in | std::ios::out | std::ios::binary);
      LinkedList<size_t> *list = new LinkedList<size_t>();
      char node_key[60];
      int compare = 1;
      size_t man_node = -1, offset = 0u, next = 0u, current = 0u, level = 0u,
         offset_left = sizeof(TreeNode::key_) + 4,
         offset_right = offset_left + sizeof(size_t),
         offset_list_head = offset_right + sizeof(size_t);

      tree.seekg(0);
      while (tree.good() && size_ != 0) {
         offset = tree.tellg();

         tree.read(node_key, sizeof(TreeNode::key_));
         compare = strcmp(wanted, node_key);

         if (compare != 0) {
            current = compare < 0 ? offset + offset_left : offset + offset_right;

            tree.seekg(current);
            tree.read(reinterpret_cast<char*>(&next), sizeof(size_t));

            if (next == 0u)
               break;

            tree.seekg(next);

         } else {

            tree.seekg(offset + offset_list_head);
            tree.read(reinterpret_cast<char*>(&next), sizeof(size_t));

            while (next != 0) {
               current = next;
               tree.seekg(current);
               tree.read(reinterpret_cast<char*>(&man_node), sizeof(size_t));
               list->push_front(man_node);

               tree.read(reinterpret_cast<char*>(&next), sizeof(size_t));
            }

            break;
         }
         ++level;
      }

      tree.close();
      return list;
   }

   LinkedList<size_t>* BinLinkedListTree::conjunctive_search(
      const char* w1, const char* w2) const {
      LinkedList<size_t> *s1 = this->search(w1);
      LinkedList<size_t> *s2 = this->search(w2);

      while (!s2->empty())
         s1->insert_sorted(s2->pop_front());

      delete s2;
      return s1;
   }

   LinkedList<size_t>* BinLinkedListTree::disjunctive_search(
      const char* w1, const char* w2) const {
      LinkedList<size_t> *s1 = search(w1);
      LinkedList<size_t> *s2 = search(w2);
      LinkedList<size_t> *list = new LinkedList<size_t>();

      size_t aux;
      while (!s2->empty()) {
         aux = s2->pop_front();
         if (s1->contains(aux))
            list->insert_sorted(aux);
      }

      delete s1;
      delete s2;
      return list;
   }

   bool BinLinkedListTree::empty() const {
      return size_ == 0u;
   }

   size_t BinLinkedListTree::size() const {
      return size_;
   }

   size_t BinLinkedListTree::depth() const {
      return depth_;
   }

   size_t BinLinkedListTree::fileSize() const {
      struct stat st;
      if (stat("./second_tree.dat", &st) != 0)
         throw std::out_of_range("MANPAGE SIZE OUT OF RANGE!");
      return st.st_size;
   }
}

#endif // !INCLUDED_BIN_LINKED_LIST_TREE
