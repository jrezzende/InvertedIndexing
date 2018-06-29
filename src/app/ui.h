#pragma once
#ifndef STRUCTURES_UI_H
#define STRUCTURES_UI_H

#include <cstdint>
#include <stdexcept>
#include <iostream>
#include <sstream>

namespace structures {

   class UserInterface {
   public:
      UserInterface();
      ~UserInterface()= default;

      size_t choose_option();
      std::string ask_word(const char* complement);
   };

   UserInterface::UserInterface() {
      std::cout << "Bem-vindo" << std::endl;
   }

   size_t UserInterface::choose_option() {
      size_t option = 100;
      std::string aux;

      do {
         if (option == 100)
            std::cout << "\nEscolha uma das op��es:" << std::endl;
         else
            std::cout << "\nOp��o inv�lida, escolha novamente:" << std::endl;

         std::cout << "0 : Busca por chave prim�ria (Imprime manpage)." << std::endl;
         std::cout << "1 : Busca por chave secund�ria." << std::endl;
         std::cout << "2 : Busca conjuntiva por chave secund�ria." << std::endl;
         std::cout << "3 : Busca disjuntiva por chave secund�ria." << std::endl;
         std::cout << "4 : Informa��es." << std::endl;
         std::cout << "5 : Sair." << std::endl;
         std::cout << ">> " << std::endl;
         std::cin >> aux;

         try {
            option = stoi(aux);
         }
         catch (std::invalid_argument e) {
            option = 6;
            continue;
         }
      } while (option > 5);

      return option;
   }

   std::string UserInterface::ask_word(const char* phrase) {
      std::string in;
      std::cout << phrase << std::endl;
      std::cout << ">> ";
      std::cin >> in;
      return in;
   }

}  //  namespace structures

#endif // STRUCTURES_UI_H
