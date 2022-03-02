#include <iostream>
#include <iterator>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

enum gate_name {
    NOT,
    XOR,
    AND,
    NAND,
    OR,
    NOR
};

static const unordered_map<gate_name, function<bool(bool, bool)>> gate_functions = {
        {AND,  [](bool a, bool b) { return a && b; }},
        {NAND, [](bool a, bool b) { return !(a && b); }},
        {OR,   [](bool a, bool b) { return a || b; }},
        {NOR,  [](bool a, bool b) { return !(a || b); }},
        {XOR,  [](bool a, bool b) { return a != b; }}
};

static const unordered_map<string, gate_name> gate_names = {
        {"NOT",  NOT},
        {"XOR",  XOR},
        {"AND",  AND},
        {"NAND", NAND},
        {"OR",   OR},
        {"NOR",  NOR}
};

using signal_number = uint32_t;
using input_data = unordered_map<signal_number, pair<gate_name, vector<signal_number>>>;
using transformed_data = unordered_map<signal_number, pair<size_t, unordered_multiset<signal_number>>>;
using generated_data = map<signal_number, bool>;

// Dodaje wczytaną bramkę do danych.
void add_to_data(input_data &logic_gates, signal_number out_signal, gate_name gate,
                 const vector<signal_number> &in_signals) {
    logic_gates.insert({out_signal, {gate, in_signals}});
}

// Wypisuje błąd składniowy linii.
void make_error(const string &line, size_t line_no) {
    cerr << "Error in line " << line_no << ": " << line << "\n";
}

// Sprawdza, uwzględniając liczbę sygnałów dla poszczególnych bramek, czy linia jest dobrze zapisana.
// W razie występującego błędu wypisuje go i zwraca false, wpp true.
bool check_line(const string &line, size_t line_no) {
    string signals_regex("(\\s+[1-9][0-9]{0,8})");
    string not_regex("NOT" + signals_regex + "{2}");
    string xor_regex("XOR" + signals_regex + "{3}");
    string other_regex("(AND|NAND|OR|NOR)" + signals_regex + "{3,}");
    static const regex valid_line_regex(
            "\\s*(" + not_regex + "|" + xor_regex + "|" + other_regex + ")\\s*");

    if (!regex_match(line, valid_line_regex)) {
        make_error(line, line_no);
        return false;
    }

    return true;
}

// Sprawdza nazwę bramki i czy sygnał wyjściowy nie jest już powiązany z inną bramką.
// W razie występującego błędu wypisuje go i zwraca false, wpp true.
bool check_out_signal(signal_number out_signal, const input_data &logic_gates, size_t line_no) {
    if (logic_gates.count(out_signal) != 0) {
        cerr << "Error in line " << line_no << ": signal " << out_signal
             << " is assigned to multiple outputs.\n";
        return false;
    }

    return true;
}

// Parsuje napis zapisany w linii na odpowiednią bramkę i dodaje ją do danych.
// W razie występującego błędu wypisuje go i zwraca false, wpp true.
bool parse_line(const string &line, size_t line_no, input_data &logic_gates) {
    if (!check_line(line, line_no)) {
        return false;
    }

    static const regex word_regex("(\\S+)");
    auto words_iterator = sregex_iterator(line.begin(), line.end(), word_regex);
    auto words_end = sregex_iterator();

    gate_name gate = gate_names.at(((words_iterator++)->str()));
    signal_number out_signal = stoul((words_iterator++)->str());

    if (!check_out_signal(out_signal, logic_gates, line_no)) {
        return false;
    }

    vector<signal_number> in_signals;

    while (words_iterator != words_end) {
        string match_str = (words_iterator++)->str();
        in_signals.push_back(stoul(match_str));
    }

    add_to_data(logic_gates, out_signal, gate, in_signals);
    return true;
}

// Wczytuje podane dane, zapisuje je i w razie błędu wypisuje go.
// Jeżeli wystąpi jakikolwiek error, to ustawia wartość is_error na true.
input_data read_input(bool &is_error) {
    string line;
    size_t line_no = 0;
    input_data result;

    while (getline(cin, line)) {
        line_no++;
        bool is_line_ok = parse_line(line, line_no, result);

        if (!is_line_ok && !is_error) {
            is_error = true;
        }
    }

    return result;
}

// Dodaje węzeł do grafu bez opisu wejść i wyjść.
void insert_node_without_description(signal_number signal, transformed_data &logic_gates_graph) {
    logic_gates_graph.insert({signal, {0, unordered_multiset<signal_number>()}});
}

// Dodaje wyjścia dla podanego węzła.
void insert_outs_for_node(signal_number signal, const vector<signal_number> &in_signals,
                          transformed_data &logic_gates_graph) {
    for (signal_number in_signal: in_signals) {
        if (!logic_gates_graph.contains(in_signal)) {
            insert_node_without_description(in_signal, logic_gates_graph);
        }

        logic_gates_graph.at(in_signal).second.insert(signal);
    }
}

// Przygotowuje mapę reprezentującą graf połączeń sygnałów potrzebny do sortowania topologicznego.
transformed_data prepare_for_sorting(const input_data &logic_gates) {
    transformed_data logic_gates_graph;

    for (const auto &logic_gate: logic_gates) {
        auto out_signal = logic_gate.first;

        if (!logic_gates_graph.contains(out_signal)) {
            insert_node_without_description(out_signal, logic_gates_graph);
        }

        logic_gates_graph.at(out_signal).first = logic_gate.second.second.size();
        insert_outs_for_node(out_signal, logic_gate.second.second, logic_gates_graph);
    }

    return logic_gates_graph;
}

// Wyciąga z grafu węzły, które nie mają żadnych wejść.
set<signal_number> pick_only_in_signals(transformed_data &logic_gates_graph) {
    set<signal_number> only_in_signals;

    for (const auto &logic_gate: logic_gates_graph) {
        if (logic_gate.second.first == 0) {
            only_in_signals.insert(logic_gate.first);
        }
    }

    for (const auto &logic_gate: logic_gates_graph) {
        if (only_in_signals.contains(logic_gate.first)) {
            for (signal_number out_signal: logic_gate.second.second) {
                logic_gates_graph.at(out_signal).first--;
            }
        }
    }

    for (auto only_in_signal: only_in_signals) {
        logic_gates_graph.erase(only_in_signal);
    }

    return only_in_signals;
}

// Sortuje typologicznie podany graf i zwraca kolejność przechodzenia go.
vector<signal_number> sort_topologically(transformed_data &logic_gates_graph) {
    vector<signal_number> order;

    for (const auto &logic_gate: logic_gates_graph) {
        if (logic_gate.second.first == 0) {
            order.push_back(logic_gate.first);
        }
    }

    for (size_t i = 0; i < order.size(); i++) {
        auto out_signals = logic_gates_graph.at(order.at(i)).second;

        for (auto out_signal: out_signals) {
            logic_gates_graph.at(out_signal).first--;

            if (logic_gates_graph.at(out_signal).first == 0) {
                order.push_back(out_signal);
            }
        }
    }

    return order;
}

// Generuje mapę, w której będą zapisywane kolejne kombinacje wartości sygnałów.
generated_data generate_map_for_signal_outputs(const transformed_data &logic_gates_graph) {
    generated_data signals_outputs;

    for (const auto &node: logic_gates_graph) {
        signals_outputs.insert({node.first, false});
    }

    return signals_outputs;
}

// Wpisuje wartości sygnałów dla sygnałów, które nie zależą od żadnej z bramek.
void insert_only_input_gates(generated_data &signals_outputs,
                             const set<signal_number> &only_in_signals, uint64_t number) {
    for (auto signal = only_in_signals.rbegin(); signal != only_in_signals.rend(); signal++) {
        signals_outputs.at(*signal) = (number % 2 == 1);
        number /= 2;
    }
}

// Wylicza wartość sygnału dla podanej bramki.
bool calculate(const pair<gate_name, vector<signal_number>> &gate,
               const generated_data &signals_outputs) {
    if (gate.first == NOT) {
        return !signals_outputs.at(gate.second.at(0));
    }

    bool result = signals_outputs.at(gate.second.at(0));
    function<bool(bool, bool)> gate_function = gate_functions.at(gate.first);

    for (auto it = gate.second.begin() + 1; it != gate.second.end(); it++) {
        result = gate_function(result, signals_outputs.at(*it));
    }

    return result;
}

// Wylicza wartość sygnału dla sygnałów zależnych według podanej kolejności.
void generate_other_gates(const input_data &logic_gates, generated_data &signals_outputs,
                          const vector<signal_number> &order) {
    for (signal_number signal: order) {
        signals_outputs.at(signal) = calculate(logic_gates.at(signal), signals_outputs);
    }
}

// Wypisuje wyliczoną kombinacje wartości sygnałów.
void write_combination(const generated_data &signals_outputs) {
    for (auto signal_output: signals_outputs) {
        cout << (signal_output.second ? 1 : 0);
    }

    cout << "\n";
}

// Podnosi liczbę 2 do podanej potęgi.
uint64_t exp_2(uint8_t n) {
    uint64_t result = 1;

    for (uint8_t i = 0; i < n; i++) {
        result <<= 1;
    }

    return result;
}

// Wylicza i wypisuje wszystkie kombinacje wartości sygnałów.
// W razie cyklu wypisuje odpowiedni error i kończy pracę.
void calculate_and_write_output(const input_data &logic_gates) {
    transformed_data logic_gates_graph = prepare_for_sorting(logic_gates);
    generated_data signals_outputs = generate_map_for_signal_outputs(logic_gates_graph);
    set<signal_number> only_in_signals = pick_only_in_signals(logic_gates_graph);
    vector<signal_number> order = sort_topologically(logic_gates_graph);

    if (order.size() != logic_gates_graph.size()) {
        cerr << "Error: sequential logic analysis has not yet been implemented.\n";
        return;
    }

    uint64_t limit = exp_2(only_in_signals.size());

    for (uint64_t i = 0; i < limit; i++) {
        insert_only_input_gates(signals_outputs, only_in_signals, i);
        generate_other_gates(logic_gates, signals_outputs, order);
        write_combination(signals_outputs);
    }
}

int main() {
    bool is_error = false;
    input_data logic_gates = read_input(is_error);

    if (!is_error) {
        calculate_and_write_output(logic_gates);
    }

    return 0;
}
