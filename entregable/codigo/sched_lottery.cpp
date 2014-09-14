#include "sched_lottery.h"
#include <stdlib.h>

using namespace std;

const int QUANTUM_TOTAL = 10;

SchedLottery::SchedLottery(vector<int> argn) {
  // FCFS recibe la cantidad de cores.

	for (int i = 0; i < argn[0]; i++){
		quantumActual.push_back(QUANTUM_TOTAL);
	}

	srand(argn[1]);
}

SchedLottery::~SchedLottery() {
}

void SchedLottery::load(int pid) {
  load(pid, 0);
}

void SchedLottery::load(int pid, int deadline) {
	// Lo agrego a la lista de procesos
	process.push_back(pid);
	int nuevoTotal = process.size();

	tickets = QUANTUM_TOTAL * nuevoTotal * 1000;
	int ticketsProceso = tickets/nuevoTotal;

	processTickets.push_back(ticketsProceso);

	for (int i = 0; i < nuevoTotal; i++) {
		processTickets[i] = ticketsProceso;
	}
}

void SchedLottery::unblock(int pid) {
	// Por ahora lo vuelvo a cargar de nuevo como un proceso nuevo
	// Se le asignara la cantidad de tickets proporcional
	load(pid);
}

int SchedLottery::tick(int cpu, const enum Motivo m) {
	switch (m) {
		case EXIT:
			return next(cpu);
			break;
		case BLOCK:
			return next(cpu);
			break;
		case TICK:
			if (current_pid(cpu) == IDLE_TASK) {
				if (!process.empty()) {
					return next(cpu);
				} else {
					return IDLE_TASK;
				}
			} else {
				quantumActual[cpu]--;
				if (quantumActual[cpu] == 0) {
					load(current_pid(cpu));
					return next(cpu);
				} else {
					return current_pid(cpu);
				}
			}
			break;
	}
}

void SchedLottery::sacar(int pid) {
	int length = process.size(), i;

	for (i = 0; i < length; i++) {
		if (process[i] == pid) {
			break;
		}
	}

	process.erase(process.begin() + i);
	processTickets.erase(processTickets.begin() + i);

	int nuevoTotal = process.size();

	if (nuevoTotal > 0) {
		tickets = QUANTUM_TOTAL * nuevoTotal * 1000;
		int ticketsProceso = tickets/nuevoTotal;

		for (i = 0; i < nuevoTotal; i++) {
			processTickets[i] = ticketsProceso;
		}
	}
}

int SchedLottery::next(int cpu) {
	int ticketGanador, procesoGanador, suma = -1, length = processTickets.size();
	
	ticketGanador = rand()%(tickets + 1);
	quantumActual[cpu] = QUANTUM_TOTAL;

	if (length > 0) {
		for (int i = 0; i < length; i++) {
			suma += processTickets[i];

			if (suma >= ticketGanador) {
				procesoGanador = process[i];
				break;
			}
		}
		// lo saco de la lista de procesos disponibles
		sacar(procesoGanador);
		return procesoGanador;
	} else  {
		return IDLE_TASK;
	}

}