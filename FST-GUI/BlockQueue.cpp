#include "FST-GUI.hpp"
#include "utils.hpp"

BlockQueue::BlockQueue(FST_GUI* f) {
    fst_gui = f;
}

bool BlockQueue::addBlockToQueue(SaveData newBlock) {
    bool uniqueBlock = true;

    for (SaveData block : this->queue) {
        if (compareBlocks(&newBlock, &block)) {
            uniqueBlock = false;
            break;
        }
    }

    if (uniqueBlock) {
        this->queue.emplace_back(newBlock);
    }

    return uniqueBlock;
}

bool BlockQueue::addBlockToQueue() {
    SaveData newBlock = fst_gui->saveStruct;

    return addBlockToQueue(newBlock);
}

SaveData BlockQueue::getNextBlockInQueue() {
    return *this->queue.begin();
}

bool BlockQueue::queueEmpty() {
    return this->queue.empty();
}

SaveData BlockQueue::removeBlockFromQueue(int index) {
    std::list<SaveData>::iterator iter = this->queue.begin();

    for (int i = 0; i < index; i++) ++iter;

    SaveData removedBlock = *iter;
    this->queue.erase(iter);

    return removedBlock;
}

void BlockQueue::clearQueue(bool removeFirst) {
    if (removeFirst) {
        this->queue.clear();
    }
    else {
        std::list<SaveData>::iterator iter = this->queue.begin();
        iter++;
        this->queue.erase(iter, this->queue.end());
    }
}

void BlockQueue::getQueueStrings(wxArrayString& queueStrings, bool running) {
    queueStrings.clear();

    const int string_float_precision = 6;

    int idx = running ? 0 : 1;

    for (SaveData block : this->queue) {
        std::string s = (idx == 0 ? "Running" : std::to_string(idx)) + ") X = [" + float2string(block.xMin, string_float_precision) + " " + float2string(block.xMax, string_float_precision) + "], Y = [" + float2string(block.yMin, string_float_precision) + " " + float2string(block.yMax, string_float_precision) + "], Z = [" + float2string(block.zMin, string_float_precision) + " " + float2string(block.zMax, string_float_precision) + "], Platform = (" + (block.platformOption == 0 ? "-1945" : "-2866") + ", -3225, -715), Size = " + std::to_string(block.xSamples) + "x" + std::to_string(block.ySamples) + "x" + std::to_string(block.zSamples);
        queueStrings.push_back(s);
        idx++;
    }
}

bool compareBlocks(SaveData* s1, SaveData* s2) {
    return s1->outputDirectory == s2->outputDirectory
        && s1->platformOption == s2->platformOption
        && s1->xMax == s2->xMax
        && s1->xMin == s2->xMin
        && s1->xSamples == s2->xSamples
        && s1->yMax == s2->yMax
        && s1->yMin == s2->yMin
        && s1->ySamples == s2->ySamples
        && s1->zMax == s2->zMax
        && s1->zMin == s2->zMin
        && s1->zSamples == s2->zSamples
        && s1->zModeOption == s2->zModeOption
        && s1->cudaExecutablePath == s2->cudaExecutablePath
        && s1->syclExecutablePath == s2->syclExecutablePath
        && s1->gpuDeviceID == s2->gpuDeviceID
        && s1->gpuModeSelected == s2->gpuModeSelected
        && s1->maxThreads == s2->maxThreads
        && s1->MAX_PLAT_SOLUTIONS == s2->MAX_PLAT_SOLUTIONS
        && s1->MAX_UPWARP_SOLUTIONS == s2->MAX_UPWARP_SOLUTIONS
        && s1->MAX_SK_PHASE_ONE == s2->MAX_SK_PHASE_ONE
        && s1->MAX_SK_PHASE_TWO_A == s2->MAX_SK_PHASE_TWO_A
        && s1->MAX_SK_PHASE_TWO_B == s2->MAX_SK_PHASE_TWO_B
        && s1->MAX_SK_PHASE_TWO_C == s2->MAX_SK_PHASE_TWO_C
        && s1->MAX_SK_PHASE_TWO_D == s2->MAX_SK_PHASE_TWO_D
        && s1->MAX_SK_PHASE_THREE == s2->MAX_SK_PHASE_THREE
        && s1->MAX_SK_PHASE_FOUR == s2->MAX_SK_PHASE_FOUR
        && s1->MAX_SK_PHASE_FIVE == s2->MAX_SK_PHASE_FIVE
        && s1->MAX_SK_PHASE_SIX == s2->MAX_SK_PHASE_SIX
        && s1->MAX_SK_UPWARP_SOLUTIONS == s2->MAX_SK_UPWARP_SOLUTIONS
        && s1->MAX_SPEED_SOLUTIONS == s2->MAX_SPEED_SOLUTIONS
        && s1->MAX_10K_SOLUTIONS == s2->MAX_10K_SOLUTIONS
        && s1->MAX_SLIDE_SOLUTIONS == s2->MAX_SLIDE_SOLUTIONS
        && s1->MAX_BD_SOLUTIONS == s2->MAX_BD_SOLUTIONS
        && s1->MAX_DOUBLE_10K_SOLUTIONS == s2->MAX_DOUBLE_10K_SOLUTIONS
        && s1->MAX_BULLY_PUSH_SOLUTIONS == s2->MAX_BULLY_PUSH_SOLUTIONS
        && s1->MAX_SQUISH_SPOTS == s2->MAX_SQUISH_SPOTS
        && s1->MAX_STRAIN_SETUPS == s2->MAX_STRAIN_SETUPS;
}