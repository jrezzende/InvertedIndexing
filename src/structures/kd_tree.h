#pragma once
#ifndef INCLUDED_KD_TREE_H
#define INCLUDED_KD_TREE_H

#include <cstdint>
#include <stdexcept>
#include <fstream>
#include <cstdio>
#include <sys/stat.h>

#include "linked_list.h"
#include "linked_stack.h"

using namespace std;

namespace structures {
   /**CLASS KDTREE*/
   class KDTree {
   public:
      ~KDTree() = default;
      KDTree();

      int insert(const char* primary, const size_t secondary, char* manpage);

      bool empty() const;
      size_t size() const;
      size_t depth() const;
      size_t file_size() const;

      char* search_primary_key(const char* required);
      string return_primary_key(const size_t required);
      LinkedList<string>* return_primary_key(LinkedList<size_t> *wanted_list);


   private:
      class Node {
      public:
         ~Node()= default;
         Node()= default;

         Node(const char* primary, const size_t secondary, char* manpage) {
            strcpy(primary_, primary);
            secondary_ = secondary;
            strcpy(manpage_, manpage);
         }

         size_t size() {
            size_t aux = sizeof(primary_) + sizeof(secondary_)
               + sizeof(left_) + sizeof(right_) + secondary_ + 10;
            return aux;
         }

         char primary_[50]{ "-" };
         size_t secondary_{ 0u },
            left_{ 0u },
            right_{ 0u };
         char manpage_[139718]{ "&" };
      };

      class Route {
      public:
         ~Route()= default;
         Route()= default;
         Route(const size_t offset, const size_t level) :
            offset_tree_{ offset },
            level_{ level }
         {}

         Route& operator=(const Route& other) {
            if (this != &other) {
               this->offset_tree_ = other.offset_tree_;
               this->level_ = other.level_;
            }
            return *this;
         }

         size_t offset_tree_{ 0u },
            level_{ 0u };
      };

      size_t depth_{ 0u },
         size_{ 0u };
   };

   KDTree::KDTree() {
      fstream tree("./first_tree.dat", ios::in | ios::out | ios::binary | ios::trunc);
      tree.close();
   }

   int KDTree::insert(const char* key_1,
      const size_t key_2, char* manpage) {
      fstream tree("./first_tree.dat", ios::in | ios::out | ios::binary);

      char node_key_1[50];
      size_t node_key_2;
      int compare = 1;
      size_t offset = 0u, son = 0u, level = 0u, father_son = 0u,
         offset_secondary = sizeof(Node::primary_) + 6,
         offset_left = offset_secondary + sizeof(size_t),
         offset_right = offset_left + sizeof(size_t);

      tree.seekg(0);
      while (tree.good() && size_ != 0) {
         offset = tree.tellg();

         if (level % 2 == 0) {
            tree.read(node_key_1, sizeof(Node::primary_));
            compare = strcmp(key_1, node_key_1);
         }
         else {
            tree.seekg(offset + offset_secondary);
            tree.read(reinterpret_cast<char*>(&node_key_2), sizeof(size_t));
            compare = key_2 - node_key_2;
         }

         if (compare != 0) {
            father_son = compare < 0 ? offset + offset_left : offset + offset_right;

            tree.seekg(father_son);
            tree.read(reinterpret_cast<char*>(&son), sizeof(size_t));

            if (son == 0u)
               break;

            tree.seekg(son);

         } else {

            if (level % 2 == 0) {
               tree.seekg(offset + offset_secondary);
               tree.read(reinterpret_cast<char*>(&node_key_2), sizeof(size_t));
               compare = key_2 - node_key_2;
            }
            else {
               tree.seekg(offset);
               tree.read(node_key_1, sizeof(Node::primary_));
               compare = strcmp(key_1, node_key_1);
            }

            if (compare == 0)
               break;

            father_son = compare < 0 ? offset + offset_left : offset + offset_right;
            tree.seekg(father_son);
            tree.read(reinterpret_cast<char*>(&son), sizeof(size_t));

            if (son == 0u)
               break;

            tree.seekg(son);
         }
         ++level;
      }
      ++level;

      if (compare != 0) {
         Node *tnode = new Node(key_1, key_2, manpage);
         if (tnode == nullptr)
            throw std::out_of_range("Full tree!");

         tree.seekp(0, ios::end);
         son = tree.tellp();

         if (size_ != 0) {
            tree.seekp(father_son);
            tree.write(reinterpret_cast<char*>(&son), sizeof(size_t));
         }

         tree.seekp(son);
         tree.write(reinterpret_cast<char*>(tnode), tnode->size());
         ++size_;
         delete tnode;
      }

      depth_ = level > depth_ ? level : depth_;
      tree.close();
      return compare == 0u ? -1 : son;
   }

   char* KDTree::search_primary_key(const char* wanted) {
      LinkedStack<Route> routes;
      char node_key_1[50];
      size_t node_key_2;
      int compare = 1;
      size_t offset = 0u, son = 0u, level = 0u,
         offset_secondary = sizeof(Node::primary_) + 6,
         offset_left = offset_secondary + sizeof(size_t),
         offset_right = offset_left + sizeof(size_t),
         offset_manpage = offset_right + sizeof(size_t);

      ifstream tree("./first_tree.dat", std::ios_base::app | ios::binary);
      tree.seekg(0);

      while (tree.good()) {
         offset = tree.tellg();

         tree.read(node_key_1, sizeof(Node::primary_));
         compare = strcmp(wanted, node_key_1);

         if (compare == 0) {
            tree.seekg(offset + offset_secondary);
            tree.read(reinterpret_cast<char*>(&node_key_2), sizeof(size_t));

            char *manpage = new char[node_key_2];
            tree.seekg(offset + offset_manpage);
            tree.read(manpage, node_key_2);
            return manpage;
         }

         if (level % 2 == 0) {
            if (compare < 0)
               son = offset + offset_left;
            else
               son = offset + offset_right;

            tree.seekg(son);
            tree.read(reinterpret_cast<char*>(&son), sizeof(size_t));

         } else {
            tree.seekg(offset + offset_right);
            tree.read(reinterpret_cast<char*>(&son), sizeof(size_t));
            if (son != 0) {
               Route way(son, level + 1);
               routes.push(way);
            }

            tree.seekg(offset + offset_left);
            tree.read(reinterpret_cast<char*>(&son), sizeof(size_t));
         }

         if (son != 0) {
            tree.seekg(son);
            ++level;
         }
         else {
            try {
               Route way = routes.pop();
               tree.seekg(way.offset_tree_);
               level = way.level_;
            }
            catch (std::out_of_range error) {
               break;
            }
         }
      }

      return nullptr;
   }

   string KDTree::return_primary_key(const size_t wanted) {
      char node_key[50];
      ifstream tree("./first_tree.dat", std::ios_base::app | ios::binary);
      tree.seekg(wanted);
      tree.read(node_key, sizeof(node_key));
      return string(node_key);
   }

   LinkedList<string>* KDTree::return_primary_key(LinkedList<size_t> *wanted_list) {
      LinkedList<string> *list = new LinkedList<string>();

      char node_key[50];
      ifstream tree("./first_tree.dat", std::ios_base::app | ios::binary);
      while (!wanted_list->empty()) {
         tree.seekg(wanted_list->pop_front());
         tree.read(node_key, sizeof(node_key));
         list->insert_sorted(node_key);
      }

      return list;
   }

   bool KDTree::empty() const {
      return size_ == 0u;
   }

   size_t KDTree::size() const {
      return size_;
   }

   size_t KDTree::depth() const {
      return depth_;
   }

   size_t KDTree::file_size() const {
      struct stat st;
      if (stat("./first_tree.dat", &st) != 0)
         throw std::out_of_range("MANPAGE SIZE OUT OF RANGE!");
      return st.st_size;
   }

}  //  namespace structures

#endif // !INCLUDED_KD_TREE_H
