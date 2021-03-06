// *****************************************************************************
// * price-tick.c
// *
// * Implemented routines for managing price tick.
// *
// * Copyright 2014 by Silicon Laboratories, Inc.
// *****************************************************************************

#include "app/framework/include/af.h"
#include "price-server-tick.h"
#include "price-server.h"

static EmberAfPriceServerPendingEvents PendingPriceEvents;

void emberAfPriceClusterServerInitTick(){
  PendingPriceEvents = EMBER_AF_PRICE_SERVER_NO_PRICES_EVENT_MASK;
}

void emberAfPriceClusterClearPendingEvent( EmberAfPriceServerPendingEvents event ){
  PendingPriceEvents &= ~(event);
}


void emberAfPriceClusterScheduleTickCallback( uint8_t endpoint, EmberAfPriceServerPendingEvents event ){
  PendingPriceEvents |= event;

  // This call will execute any ready events and schedule the tick for the nearest future event.
  emberAfPriceClusterServerTickCallback( endpoint );
}

void emberAfPriceClusterServerTickCallback( uint8_t endpoint ){
  uint32_t timeNowUtc;
  uint32_t minEventDelaySec = ZCL_PRICE_CLUSTER_END_TIME_NEVER;
  uint32_t eventDelaySec;
  uint16_t minTimeEventMask = 0;
  uint32_t delayMSec;

  uint8_t ep = emberAfFindClusterServerEndpointIndex( endpoint, ZCL_PRICE_CLUSTER_ID );
  emberAfPriceClusterPrintln("Price Tick Callback, ep=%d", ep );
  if( ep == 0xFF ){
    return;
  }

  timeNowUtc = emberAfGetCurrentTime();
  UNUSED_VAR(timeNowUtc);
  UNUSED_VAR(minTimeEventMask);

  // Look at all currently pending events and determine the min delay time for each.
  if( PendingPriceEvents & EMBER_AF_PRICE_SERVER_GET_SCHEDULED_PRICES_EVENT_MASK ){
    eventDelaySec = emberAfPriceServerSecondsUntilGetScheduledPricesEvent();
    if( eventDelaySec == 0 ){
      // Execute now
      emberAfPriceClusterPrintln("Price Tick:  Get Scheduled Prices");
      emberAfPriceServerSendGetScheduledPrices( endpoint );
      // Recalculate next delay time
      eventDelaySec = emberAfPriceServerSecondsUntilGetScheduledPricesEvent();
    }
    if( eventDelaySec <= minEventDelaySec ){  // This should preempt any other events.
      minEventDelaySec = eventDelaySec;
      minTimeEventMask = EMBER_AF_PRICE_SERVER_GET_SCHEDULED_PRICES_EVENT_MASK;
    }
    if( eventDelaySec == PRICE_EVENT_TIME_NO_PENDING_EVENTS ){
      emberAfPriceClusterClearPendingEvent( EMBER_AF_PRICE_SERVER_GET_SCHEDULED_PRICES_EVENT_MASK );
    }
  }

  if( PendingPriceEvents & EMBER_AF_PRICE_SERVER_CHANGE_BILLING_PERIOD_EVENT_MASK ){
    eventDelaySec = emberAfPriceServerSecondsUntilBillingPeriodEvent( endpoint );
    if( eventDelaySec == 0 ){
      // Execute now
      emberAfPriceClusterPrintln("Price Tick:  Billing Period");
      emberAfPriceServerRefreshBillingPeriod( endpoint );
      // Recalculate next delay time
      eventDelaySec = emberAfPriceServerSecondsUntilBillingPeriodEvent( endpoint );
    }
    if( eventDelaySec < minEventDelaySec ){
      minEventDelaySec = eventDelaySec;
      minTimeEventMask = EMBER_AF_PRICE_SERVER_CHANGE_BILLING_PERIOD_EVENT_MASK;
    }
    if( eventDelaySec == PRICE_EVENT_TIME_NO_PENDING_EVENTS ){
      emberAfPriceClusterClearPendingEvent( EMBER_AF_PRICE_SERVER_CHANGE_BILLING_PERIOD_EVENT_MASK );
    }
  }


  if( PendingPriceEvents & EMBER_AF_PRICE_SERVER_CHANGE_BLOCK_PERIOD_EVENT_MASK ){
    eventDelaySec = emberAfPriceServerSecondsUntilBlockPeriodEvent( endpoint );
    if( eventDelaySec == 0 ){
      // Execute now
      emberAfPriceClusterPrintln("Price Tick:  Block Period");
      emberAfPriceServerRefreshBlockPeriod( endpoint );

      // Per SE1.2 Spec D4.4.3, 
      // PublishPrice is required at the start of a Block Period
      emberAfPluginPriceServerPriceUpdateBindings();
      
      // Recalculate next delay time
      eventDelaySec = emberAfPriceServerSecondsUntilBlockPeriodEvent( endpoint );
    }
    if( eventDelaySec < minEventDelaySec ){
      minEventDelaySec = eventDelaySec;
      minTimeEventMask = EMBER_AF_PRICE_SERVER_CHANGE_BLOCK_PERIOD_EVENT_MASK;
    }
    if( eventDelaySec == PRICE_EVENT_TIME_NO_PENDING_EVENTS ){
      emberAfPriceClusterClearPendingEvent( EMBER_AF_PRICE_SERVER_CHANGE_BLOCK_PERIOD_EVENT_MASK );
    }
  }

  if( PendingPriceEvents & EMBER_AF_PRICE_SERVER_CHANGE_CALORIFIC_VALUE_EVENT_MASK ){
    eventDelaySec = emberAfPriceServerSecondsUntilCalorificValueEvent( endpoint );
    if( eventDelaySec == 0 ){
      // Execute now
      emberAfPriceClusterPrintln("Price Tick:  Calorific Value");
      emberAfPriceServerRefreshCalorificValue( endpoint );
      // Recalculate next delay time
      eventDelaySec = emberAfPriceServerSecondsUntilCalorificValueEvent( endpoint );
    }
    if( eventDelaySec < minEventDelaySec ){
      minEventDelaySec = eventDelaySec;
      minTimeEventMask = EMBER_AF_PRICE_SERVER_CHANGE_CALORIFIC_VALUE_EVENT_MASK;
    }
    if( eventDelaySec == PRICE_EVENT_TIME_NO_PENDING_EVENTS ){
      emberAfPriceClusterClearPendingEvent( EMBER_AF_PRICE_SERVER_CHANGE_CALORIFIC_VALUE_EVENT_MASK );
    }
  }

  if( PendingPriceEvents & EMBER_AF_PRICE_SERVER_CHANGE_CO2_VALUE_EVENT_MASK ){
    eventDelaySec = emberAfPriceServerSecondsUntilCO2ValueEvent( endpoint );
    if( eventDelaySec == 0 ){
      // Execute now
      emberAfPriceClusterPrintln("Price Tick:  CO2 Value");
      emberAfPriceServerRefreshCO2Value( endpoint );
      // Recalculate next delay time
      eventDelaySec = emberAfPriceServerSecondsUntilCO2ValueEvent( endpoint );
    }
    if( eventDelaySec < minEventDelaySec ){
      minEventDelaySec = eventDelaySec;
      minTimeEventMask = EMBER_AF_PRICE_SERVER_CHANGE_CO2_VALUE_EVENT_MASK;
    }
    if( eventDelaySec == PRICE_EVENT_TIME_NO_PENDING_EVENTS ){
      emberAfPriceClusterClearPendingEvent( EMBER_AF_PRICE_SERVER_CHANGE_CO2_VALUE_EVENT_MASK );
    }
  }

  if( PendingPriceEvents & EMBER_AF_PRICE_SERVER_CHANGE_CONVERSION_FACTOR_EVENT_MASK ){
    eventDelaySec = emberAfPriceServerSecondsUntilConversionFactorEvent( endpoint );
    if( eventDelaySec == 0 ){
      // Execute now
      emberAfPriceClusterPrintln("Price Tick:  Conversion Factor");
      emberAfPriceServerRefreshConversionFactor( endpoint );
      // Recalculate next delay time
      eventDelaySec = emberAfPriceServerSecondsUntilConversionFactorEvent( endpoint );
    }
    if( eventDelaySec < minEventDelaySec ){
      minEventDelaySec = eventDelaySec;
      minTimeEventMask = EMBER_AF_PRICE_SERVER_CHANGE_CONVERSION_FACTOR_EVENT_MASK;
    }
    if( eventDelaySec == PRICE_EVENT_TIME_NO_PENDING_EVENTS ){
      emberAfPriceClusterClearPendingEvent( EMBER_AF_PRICE_SERVER_CHANGE_CONVERSION_FACTOR_EVENT_MASK );
    }
  }

  if( PendingPriceEvents & EMBER_AF_PRICE_SERVER_CHANGE_TARIFF_INFORMATION_EVENT_MASK ){
    eventDelaySec = emberAfPriceServerSecondsUntilTariffInfoEvent(endpoint);
    if( eventDelaySec == 0 ){
      // Execute now
      emberAfPriceClusterPrintln("Price Tick: Tariff Information");
      emberAfPriceServerRefreshTariffInformation( endpoint );
      // Recalculate next delay time
      eventDelaySec = emberAfPriceServerSecondsUntilTariffInfoEvent( endpoint );
    }
    if( eventDelaySec < minEventDelaySec ){
      minEventDelaySec = eventDelaySec;
      minTimeEventMask = EMBER_AF_PRICE_SERVER_CHANGE_TARIFF_INFORMATION_EVENT_MASK;
    }
    if( eventDelaySec == PRICE_EVENT_TIME_NO_PENDING_EVENTS ){
      emberAfPriceClusterClearPendingEvent(EMBER_AF_PRICE_SERVER_CHANGE_TARIFF_INFORMATION_EVENT_MASK);
    }
  }

  if( PendingPriceEvents & EMBER_AF_PRICE_SERVER_ACTIVATED_PRICE_MATRIX_EVENT_MASK ){
    eventDelaySec = emberAfPriceServerSecondsUntilActivePriceMatrixEvent(endpoint);
    if( eventDelaySec == 0 ){
      // Execute now
      emberAfPriceClusterPrintln("Price Tick: PriceMatrix Information activated");
      emberAfPriceServerRefreshPriceMatrixInformation( endpoint );
      // Recalculate next delay time
      eventDelaySec = emberAfPriceServerSecondsUntilActivePriceMatrixEvent( endpoint );
    }
    if( eventDelaySec < minEventDelaySec ){
      minEventDelaySec = eventDelaySec;
      minTimeEventMask = EMBER_AF_PRICE_SERVER_ACTIVATED_PRICE_MATRIX_EVENT_MASK;
    }
    if( eventDelaySec == PRICE_EVENT_TIME_NO_PENDING_EVENTS ){
      emberAfPriceClusterClearPendingEvent(EMBER_AF_PRICE_SERVER_ACTIVATED_PRICE_MATRIX_EVENT_MASK);
    }
  }

  if( PendingPriceEvents & EMBER_AF_PRICE_SERVER_ACTIVATED_BLOCK_THRESHOLD_EVENT_MASK ){
    eventDelaySec = emberAfPriceServerSecondsUntilActiveBlockThresholdsEvent(endpoint);
    if( eventDelaySec == 0 ){
      // Execute now
      emberAfPriceClusterPrintln("Price Tick: BlockThresholds activated");
      emberAfPriceServerRefreshBlockThresholdsInformation( endpoint );
      // Recalculate next delay time
      eventDelaySec = emberAfPriceServerSecondsUntilActiveBlockThresholdsEvent( endpoint );
    }
    if( eventDelaySec < minEventDelaySec ){
      minEventDelaySec = eventDelaySec;
      minTimeEventMask = EMBER_AF_PRICE_SERVER_ACTIVATED_BLOCK_THRESHOLD_EVENT_MASK;
    }
    if( eventDelaySec == PRICE_EVENT_TIME_NO_PENDING_EVENTS ){
      emberAfPriceClusterClearPendingEvent(EMBER_AF_PRICE_SERVER_ACTIVATED_BLOCK_THRESHOLD_EVENT_MASK);
    }
  }
  
  if( minEventDelaySec == 0 ){
    // Get Scheduled Prices wants to send each price with a 250ms delay between them (< 1 second).
    // To accommodate this behavior, any event that returns 0 for its delay time after execution
    // will be padded with a 250ms delay.
    delayMSec = MILLISECOND_TICKS_PER_QUARTERSECOND;
  }
  else if( minEventDelaySec != 0xFFFFFFFF ){
    delayMSec = (minEventDelaySec * MILLISECOND_TICKS_PER_SECOND);
  } else{
    //delayMSec = minEventDelaySec;
    return;   // Nothing left to do.
  }
  
  if (PendingPriceEvents != 0x00){
    emberAfPriceClusterPrintln("Scheduling Tick Callback in %d sec, eventBitFlag=%d, pendingEvents=%d", minEventDelaySec, minTimeEventMask, PendingPriceEvents );
  } else {
    emberAfPriceClusterPrintln("Scheduling Tick Callback in %d msec", delayMSec);
  }
  emberAfScheduleServerTick( endpoint, ZCL_PRICE_CLUSTER_ID, delayMSec );
}




