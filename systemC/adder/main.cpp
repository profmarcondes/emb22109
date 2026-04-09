#include <systemc>
#include <iostream>

using namespace std;
using namespace sc_core;
using namespace sc_dt;

SC_MODULE(HalfAdder){
	sc_in<sc_logic> a, b;
	sc_out<sc_logic> s, c;

	void doTheThing(){
		s = a ^ b;
		c = a & b;
	}

	SC_CTOR(HalfAdder){
		SC_METHOD(doTheThing);
		sensitive << a << b;
	}
};


SC_MODULE(FullAdder){
	sc_in<sc_logic> input_a;
	sc_in<sc_logic> input_b;
	sc_in<sc_logic> input_carry;
	sc_out<sc_logic> output_sum;
	sc_out<sc_logic> output_carry;

	void process(){
		output_carry = carry_h1 | carry_h2;
	}

	SC_CTOR(FullAdder): adder01("Adder01"), adder02("Adder02") {
		SC_METHOD(process);
		sensitive << carry_h1 << carry_h2;

		adder01.a(input_a);
		adder01.b(input_b);
		adder01.s(m_sum);
		adder01.c(carry_h1);

		adder02.a(input_carry);
		adder02.b(m_sum);
		adder02.s(output_sum);
		adder02.c(carry_h2);
	}

private:
	HalfAdder adder01;
	HalfAdder adder02;
	sc_signal<sc_logic> m_sum;
	sc_signal<sc_logic> carry_h1;
	sc_signal<sc_logic> carry_h2;
	
};


int sc_main (int argc, char* argv[]) {
	cout << "Hello adder !" << endl;

	sc_signal<sc_logic> in_a, in_b, in_carry;
	sc_signal<sc_logic> out_sum, out_carry;
	
	FullAdder somador("Somador01");
	somador.input_a(in_a);
	somador.input_b(in_b);
	somador.input_carry(in_carry);
	somador.output_carry(out_carry);
	somador.output_sum(out_sum);

	sc_trace_file * wf = sc_create_vcd_trace_file("trace");
	sc_trace(wf, in_a, "entrada_A");
	sc_trace(wf, in_b, "entrada_B");
	sc_trace(wf, in_carry, "entrada_CARRY");
	sc_trace(wf, out_sum, "saida_SUM");
	sc_trace(wf, out_carry, "saida_CARRY");
	

	sc_uint<3> reg = 0;
	for(int i = 0; i < 8; i++){
		in_a.write(sc_logic((bool)reg[0]));
		in_b.write(sc_logic((bool)reg[1]));
		in_carry.write(sc_logic((bool)reg[2]));
		sc_start(1, SC_NS);
		cout << "@" << sc_time_stamp() << "(A,B,CARRY_IN, SUM, CARRY_OUT)=";
		cout << in_a.read() << ",";
		cout << in_b.read() << ",";
		cout << in_carry.read() << ",";
		cout << out_sum.read() << ",";
		cout << out_carry.read() << endl;
		reg++;
	}

	sc_close_vcd_trace_file(wf);

	return 0;
}
