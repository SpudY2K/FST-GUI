#include "FST-GUI.hpp"
#include "utils.hpp"

BlockQueue::BlockQueue(FST_GUI* f) {
    fst_gui = f;
}

bool BlockQueue::addBlockToQueue(BlockData newBlock) {
    bool uniqueBlock = true;

    for (BlockData block : this->queue) {
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
    BlockData newBlock = fst_gui->saveStruct.blockData;

    return addBlockToQueue(newBlock);
}

std::list<BlockData>::iterator BlockQueue::queueBegin() {
    return this->queue.begin();
}

BlockData BlockQueue::getNextBlockInQueue() {
    return *this->queue.begin();
}

bool BlockQueue::queueEmpty() {
    return this->queue.empty();
}

int BlockQueue::queueLength() {
    return this->queue.size();
}

BlockData BlockQueue::removeBlockFromQueue(int index) {
    std::list<BlockData>::iterator iter = this->queue.begin();

    for (int i = 0; i < index; i++) ++iter;

    BlockData removedBlock = *iter;
    this->queue.erase(iter);

    return removedBlock;
}

void BlockQueue::clearQueue(bool removeFirst) {
    if (removeFirst) {
        this->queue.clear();
    }
    else {
        std::list<BlockData>::iterator iter = this->queue.begin();
        iter++;
        this->queue.erase(iter, this->queue.end());
    }
}

void BlockQueue::getQueueStrings(wxArrayString& queueStrings, bool running) {
    queueStrings.clear();

    const int string_float_precision = 6;

    int idx = running ? 0 : 1;

    for (BlockData block : this->queue) {
        std::string s = (idx == 0 ? "Running" : std::to_string(idx)) + ") X = [" + float2string(block.xMin, string_float_precision) + " " + float2string(block.xMax, string_float_precision) + "], Y = [" + float2string(block.yMin, string_float_precision) + " " + float2string(block.yMax, string_float_precision) + "], " + (block.zModeOption == 1 ? "Z" : "XZ") + " = [" + float2string(block.zMin, string_float_precision) + " " + float2string(block.zMax, string_float_precision) + "], Platform = (" + (block.platformOption == 0 ? "-1945" : "-2866") + ", -3225, -715), Size = " + std::to_string(block.xSamples) + "x" + std::to_string(block.ySamples) + "x" + std::to_string(block.zSamples);
        queueStrings.push_back(s);
        idx++;
    }
}

bool compareBlocks(BlockData* s1, BlockData* s2) {
    return s1->platformOption == s2->platformOption
        && s1->xMax == s2->xMax
        && s1->xMin == s2->xMin
        && s1->xSamples == s2->xSamples
        && s1->yMax == s2->yMax
        && s1->yMin == s2->yMin
        && s1->ySamples == s2->ySamples
        && s1->zMax == s2->zMax
        && s1->zMin == s2->zMin
        && s1->zSamples == s2->zSamples
        && s1->zModeOption == s2->zModeOption;
}