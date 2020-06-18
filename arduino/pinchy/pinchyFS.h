/*============================================================================*\
 * Pinchy - PinchyFS
 * Peter Eldred 2020-06
 * 
 * This class manages all of the file system methods required for Pinchy
\*============================================================================*/
#ifndef PINCHYFS_H
#define PINCHYFS_H

class PinchyFS {
  public:
    PinchyFS();
    bool init();

  private:
};

extern PinchyFS pinchyFS;

#endif
