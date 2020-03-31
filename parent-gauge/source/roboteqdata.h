#ifndef ROBOTEQ_DATA_H
#define ROBOTEQ_DATA_H


struct RoboteqData {
  unsigned int ccs = 1;
  unsigned int n;
  unsigned int xx = 0;
  int index;
  int subindex;
  int data;
};


RoboteqData::populate(const QByteArray buffer) {
  assert(buffer.length() == 8);

  this->data = buffer[4] << 24 | buffer[5] << 16 | buffer[6] << 8 | buffer[7];
  this->subindex = buffer[3];
  this->index = buffer[1] << 8 | buffer[2];

  // PROCESS BYTE 0
  char byte0 = buffer[0];
  this->n = byte0 & 0x6;

}








#endif
