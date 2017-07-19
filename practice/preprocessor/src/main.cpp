#include <cstdio>
#include <cassert>
#include <vector>
#include <utility>
#include <cstdlib>
#include <ctime>

#include "myassert.h"
#include "mylog.h"
#include "instructions.h"

const char* instruction_name(InstructionCode code)
{
    (void)code;
    // TODO
    return "I don't know";
}

const char* instruction_description(InstructionCode code)
{
    (void)code;
    // TODO
    return "I don't know";
}

std::vector<unsigned char> gen_rnd_instrs(const size_t instr_cnt)
{
    std::vector<unsigned char> buffer;
    // TODO
    return std::move(buffer);
}

void log_instrs(const std::vector<unsigned char> &instrs)
{
    // TODO
    (void)instrs;
}

void exec_instrs(const std::vector<unsigned char> &instrs)
{
    (void)instrs;
    // TODO this function should interpret instrs stream.
    // instrs stream parsing is similar to the one done in log_instrs
}

int main()
{
    std::srand(std::time(0));
    using ::LOG_LEVELS;
    // Part 1
    bool assertion1_evaluated = false;
    myassert(assertion1_evaluated = true);
    assert((CONFIG_DEBUG && assertion1_evaluated)
            || (!CONFIG_DEBUG && !assertion1_evaluated));

    //myassert(1 == 4);

    // Part 2
    LOG(LOG_LEVELS::INFO, "==============================");
    LOG(LOG_LEVELS::DEBUG, "%d %d %d %s", 1, 2, 3, "debug");
    LOG(LOG_LEVELS::INFO, "%d %d %s", 1, 2, "info");
    LOG(LOG_LEVELS::WARN, "%d %s", 1, "warning");
    LOG(LOG_LEVELS::ERROR, "%s", "error!");

    // Part 3
    LOG(LOG_LEVELS::INFO, "==============================");
    LOG(LOG_LEVELS::INFO, "%s %s", instruction_name(INST_DADD),
            instruction_description(INST_DADD));
    LOG(LOG_LEVELS::INFO, "%s %s", instruction_name(INST_IPRINT),
            instruction_description(INST_IPRINT));

    // Part 4
    auto inst_stream = gen_rnd_instrs(20);
    log_instrs(inst_stream);

    // Part 5
    LOG(LOG_LEVELS::INFO, "==============================");
    std::vector<unsigned char> super_code = {
        INST_ILOAD, 0xff, 0, 0, 0, 0, 0, 0, 0,
        INST_ILOAD, 0, 0, 0xdd, 0, 0, 0, 0, 0,
        INST_IADD,
        INST_ILOAD, 0xff, 0, 0, 0, 0, 0, 0, 0,
        INST_IADD,
        INST_IPRINT
    }; // 0xff + 0xdd0000 + 0xff = 0xdd01fe
    log_instrs(super_code);
    exec_instrs(super_code);

    return 0;
}
