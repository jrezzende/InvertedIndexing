#pragma once
#ifndef STRUCTURES_HANDLER_H
#define STRUCTURES_HANDLER_H

#include <cstdint>
#include <stdexcept>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>

#include "array_list.h"
#include "linked_list.h"

namespace structures {

   class WordHandler {
   public:
      WordHandler();
      ~WordHandler()= default;

      std::string clean_primary_key(std::string key);
      LinkedList<std::string>* treatment(std::ifstream &file);

   private:
      ArrayList<std::string> ignored_words{ 250 };
      std::string separations{ " '`^,.-+:;=<>[](){}|/_%*&$#@!?0123456789\"\f\n\r\t\v\\" };
      char *token{ nullptr };
   };

   WordHandler::WordHandler() {
      std::ifstream file("./ignored_words.txt", std::ios::in);
      std::string word;

      if (!file)
         throw std::out_of_range("Error: open ./ignored_words.txt");

      file.seekg(0);
      while (file >> word) {
         token = strtok(&word[0], separations.c_str());

         while (token != NULL) {
            std::string temp(token);
            ignored_words.push_back(temp);
            token = strtok(NULL, separations.c_str());
         }
      }

   }

   std::string WordHandler::clean_primary_key(std::string key) {
      return key.substr(11, key.size() - 15);
   }

   LinkedList<std::string>* WordHandler::treatment(std::ifstream &file) {
      std::string word;
      LinkedList<std::string> *list = new LinkedList<std::string>();

      file.seekg(0);
      while (file >> word) {
         token = strtok(&word[0], separations.c_str());

         while (token != NULL) {
            std::string temp(token);
            transform(temp.begin(), temp.end(), temp.begin(), ::tolower);

            if (!ignored_words.contains(temp) && !list->contains(temp))
               list->push_front(temp);
            token = strtok(NULL, separations.c_str());
         }
      }

      return list;
   }

}  //  namespace structures

#endif
