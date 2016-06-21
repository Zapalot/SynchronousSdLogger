/*
  CircularBuffer.h - circular buffer library for Arduino.
  Copyright (c) 2009 Hiroki Yagita.
  Permission is hereby granted, free of charge, to any person obtaining
  a copy of this software and associated documentation files (the
  'Software'), to deal in the Software without restriction, including
  without limitation the rights to use, copy, modify, merge, publish,
  distribute, sublicense, and/or sell copies of the Software, and to
  permit persons to whom the Software is furnished to do so, subject to
  the following conditions:
  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

 //Modified by Felix Bonowski, 2016
#ifndef CIRCULARBUFFER_h
#define CIRCULARBUFFER_h
#include <inttypes.h>

template <typename T, uint8_t Size>
class CircularBuffer {
public:
  enum {
    Empty = 0,
    Half = Size / 2,
    Full = Size,
  };

  CircularBuffer() :
    writePointer(buffer), readPointer(buffer), lastElementPointer(buffer+Size),  nElementsInBuffer(0) {}
    
  ~CircularBuffer() {}
  
  void push(T value) {
    *writePointer++ = value;
    nElementsInBuffer++;
    if (writePointer == lastElementPointer) writePointer = buffer;
  }
  
  T pop() {
    T result = *readPointer++;
    nElementsInBuffer--;
    if (readPointer == lastElementPointer) readPointer = buffer;
    return result;
  }
  
  int remain() const {
    return nElementsInBuffer;
  }

private:
  T buffer[Size];
  T *writePointer;
  T *readPointer;
  T *lastElementPointer;
  uint8_t nElementsInBuffer;
};

#endif
