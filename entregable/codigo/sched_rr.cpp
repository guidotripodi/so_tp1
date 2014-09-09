#include <vector>
#include <queue>
#include "sched_rr.h"
#include "basesched.h"
#include <iostream>

using namespace std;


SchedRR::SchedRR(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
	for (int i = 0; i < argn[0]; i++) {
		quantum[i] = argn[i + 1];
		quantumActual[i] = argn[i + 1];
	}
}

SchedRR::~SchedRR() {

}


void SchedRR::load(int pid) {
	q.push(pid); // llegó una tarea nueva
}

void SchedRR::unblock(int pid) {
	bloqueados.remove(pid);
	q.push(pid);
}

int SchedRR::tick(int cpu, const enum Motivo m) {
	int sig;

	switch (m) {
		case EXIT:
			quantumActual[cpu] = quantum[cpu];

			// Si el pid actual terminó, sigue el próximo.
			if (q.empty()) sig = IDLE_TASK;
			else {
				sig = q.front(); q.pop();
			}
			break;
		case BLOCK:
			quantumActual[cpu] = quantum[cpu];
			bloqueados.push_back(current_pid(cpu));

			sig = q.front();
			q.pop();
			break;
		case TICK:
			if (current_pid(cpu) == IDLE_TASK) {
				if (!q.empty()) {
					sig = q.front(); q.pop();
					quantumActual[cpu] = quantum[cpu];
				} else {
					sig = IDLE_TASK;
				}
			} else {
				quantumActual[cpu]--;
				if (quantumActual[cpu] == 0) {
					q.push(current_pid(cpu));
					
					sig = q.front(); q.pop();
					quantumActual[cpu] = quantum[cpu];
				} else {
					sig = current_pid(cpu);
				}
			}
			break;
		default:
			sig = current_pid(cpu);
	}

	return sig;
}