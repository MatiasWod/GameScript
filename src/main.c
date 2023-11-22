#include "backend/code-generation/generator.h"
#include "backend/support/logger.h"
#include "backend/support/shared.h"
#include "frontend/syntactic-analysis/bison-parser.h"
#include <stdio.h>
#include <stdlib.h>
#include "backend/support/utils.h"
// Estado de la aplicación.
CompilerState state;

// Punto de entrada principal del compilador.
const int main(const int argumentCount, const char ** arguments) {
	// Inicializar estado de la aplicación.
	state.program = NULL;
	state.errors = NULL;
	state.last_error = NULL;
	state.error_count = 0;
	state.succeed = false;
	state.game_generation = false;

	// Checkeamos que unicamente se ponga el path del archivo a correr, sin argumentos

	if (argumentCount == 1) {
		state.game_generation = true;
	}
	else {
		LogError("Cantidad de argumentos invalida. Se esperaba unicamente el path del archivo a correr.");
		return -1;
	}

	// Mostrar parámetros recibidos por consola.
	for (int i = 0; i < argumentCount; ++i) {
		LogInfo("Argumento %d: '%s'", i, arguments[i]);
	}

	// Compilar el programa de entrada.
	LogInfo("Compilando...\n");
	const int result = yyparse();
	switch (result) {
		case 0:
			// La variable "succeed" es la que setea Bison al identificar el símbolo
			// inicial de la gramática satisfactoriamente.
			if (state.succeed) {
				LogInfo("La compilacion fue exitosa.");
				Generator(state.program);
			}
			else {
				LogError("Se produjo un error en la aplicacion.");
				return -1;
			}
			break;
		case 1:
			LogError("Bison finalizo debido a un error de sintaxis.");
			while (state.errors != NULL) {
				printf("%s", state.errors->message);
				state.errors = state.errors->next;
			}
			break;
		case 2:
			LogError("Bison finalizo abruptamente debido a que ya no hay memoria disponible.");
			break;
		default:
			LogError("Error desconocido mientras se ejecutaba el analizador Bison (codigo %d).", result);
	}
	FreeAll();
	LogInfo("Fin.");
	return result;
}


