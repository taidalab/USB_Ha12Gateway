/*
 * File: gp-util.c
 * Description: GP utilities common to SOC, Host, and NCP
 *
 * Author(s): Jeffrey Rosenberger, jeffrey.rosenberger@silabs.com
 *
 * Copyright 2015 Silicon Laboratories, Inc.                                *80*
 */
#include PLATFORM_HEADER
#include "stack/include/ember-types.h"
#include "stack/core/ember-stack.h"
#include "stack/include/gp-types.h"
//#include "gp.h"

bool emGpAddressMatch(const EmberGpAddress *a1, const EmberGpAddress *a2)
{
  if (a1->applicationId == EMBER_GP_APPLICATION_SOURCE_ID
      && a2->applicationId == EMBER_GP_APPLICATION_SOURCE_ID) {
    if (a1->id.sourceId == a2->id.sourceId)
      return true;
  } else if (a1->applicationId == EMBER_GP_APPLICATION_IEEE_ADDRESS
             && a2->applicationId == EMBER_GP_APPLICATION_IEEE_ADDRESS) {
    if (!MEMCOMPARE(a1->id.gpdIeeeAddress, a2->id.gpdIeeeAddress, EUI64_SIZE)) {
      if (a1->endpoint == a2->endpoint) //TODO handle endpoints 0 and FF
        return true;
    }
  }
  return false;
}

EmberNodeId emGpdAlias(EmberGpAddress *addr)
{
  EmberNodeId alias;
  if (addr->applicationId == EMBER_GP_APPLICATION_SOURCE_ID) {
    alias = addr->id.sourceId & 0xFFFF;
    if (alias == 0x0000 || ((alias & 0xFFF8) == 0xFFF8)) {
      alias = alias ^ (addr->id.sourceId >> 16);
      if (alias == 0x0000 || ((alias & 0xFFF8) == 0xFFF8)) {
        if ((addr->id.sourceId & 0xFFFF) == 0x0000)
          alias = 0x0007;
        else
          alias -= 0x0008;
      }
    }
  }
  return alias;
}
