#pragma once

/** <copyright github.com/jrezzende> */

#ifndef STRUCTURES_LINKED_LIST_H
#define STRUCTURES_LINKED_LIST_H

#include <cstdint>
#include <stdexcept>

namespace structures {

   template<typename T>
   /**CLASS LINKEDLIST*/
   class LinkedList {
   public:
      LinkedList() {
         head = nullptr;
         size_ = 0;
      }
 
      ~LinkedList() {
         clear();
      }

      void clear() {
         while (!empty()) {
            pop_front();
         }
      }

      void push_front(const T& data) {
         Node* new_node = new Node(data, head);
         head = new_node;
         size_++;
      }

      void push_back(const T& data) {
         insert(data, size_);
      }

      void insert(const T& data, std::size_t index) {
         Node* prev;
         if (index > size_) {
            throw std::out_of_range("ERRO DE INDICE");
         }
         else if (index == 0 || empty()) {
            push_front(data);
         }
         else {
            Node* new_node = new Node(data);
            if (new_node == nullptr)
               throw std::out_of_range("ERRO DE MEMORIA!");
            prev = head;
            for (auto i = 1u; i < index; i++)
               prev = prev->next();
            new_node->next(prev->next());
            prev->next(new_node);
            size_++;
         }
      }
      void insert_sorted(const T& data) {
         Node* current;
         int index = 0;
         if (empty())
            return push_front(data);
         current = head;
         while (current->next() != nullptr && data > current->data()) {
            current = current->next();
            index++;
         }
         if (data > current->data())
            return insert(data, index + 1);
         else
            return insert(data, index);
      }

      T& at(std::size_t index) {
         if (empty())
            throw std::out_of_range("LISTA VAZIA!");
         Node* current = head;
         for (int i = 0; i < index; i++) {
            if (current->next() == nullptr) {
               throw std::out_of_range("ERRO DE INDICE!");
            }
            current = current->next();
         }
         return current->data();
      }
      T pop(std::size_t index) {
         if (index >= size_) {
            throw std::out_of_range("ERRO DE INDICE");
         }
         else if (empty()) {
            throw std::out_of_range("LISTA VAZIA");
         }
         else if (size_ == 1) {
            Node* singleCase = head;
            head = nullptr;
            auto temp = singleCase->data();
            delete singleCase;
            size_--;
            return temp;
         }
         Node* current = head;
         Node* toDelete;
         for (int i = 0; i < index - 1; i++)
            current = current->next();
         toDelete = current->next();
         current->next(toDelete->next());
         auto temp = toDelete->data();
         size_--;
         delete toDelete;
         return temp;
      }

      T pop_front() {
         if (empty()) {
            throw std::out_of_range("LISTA VAZIA!");
         }
         else if (size_ == 1) {
            Node* singleCase = head;
            head = nullptr;
            auto temp = singleCase->data();
            size_--;
            delete singleCase;
            return temp;
         }
         Node* toDelete = head;
         auto temp = toDelete->data();
         head = toDelete->next();
         size_--;
         delete toDelete;
         return temp;
      }

      T pop_back() {
         return pop(size_ - 1);
      }

      void remove(const T& data) {
         pop(find(data));
      }

      bool empty() const {
         return (size_ == 0);
      }

      bool contains(const T& data) const {
         Node* current = head;
         if (empty())
            throw std::out_of_range("LISTA VAZIA");
         for (int i = 0; i < size_; i++) {
            if (current->data() == data)
               return true;
            current = current->next();
         }
         return false;
      }
      std::size_t find(const T& data) const {
         Node* current = head;
         if (empty())
            throw std::out_of_range("LISTA VAZIA");
         for (int i = 0; i < size_; i++) {
            if (current->data() == data)
               return i;
            current = current->next();
         }
         return size_;
      }

      std::size_t size() const {
         return size_;
      }

   private:
      class Node {
      public:
         explicit Node(const T& data) :
            data_{ data }
         {}

         Node(const T& data, Node* next) :
            data_{ data },
            next_{ next }
         {}

         T& data() {
            return data_;
         }

         const T& data() const {
            return data_;
         }

         Node* next() { 
            return next_;
         }

         const Node* next() const {
            return next_;
         }

         void next(Node* node) {
            next_ = node;
         }

      private:
         T data_;
         Node* next_{ nullptr };
      };

      Node* end() {
         auto it = head;
         for (auto i = 1u; i < size(); ++i) {
            it = it->next();
         }
         return it;
      }

      Node* head{ nullptr };
      std::size_t size_{ 0u };
   };
}  //  namespace structures


#endif // STRUCTURES_LINKED_LIST_H
