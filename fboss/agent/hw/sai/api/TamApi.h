// Copyright 2004-present Facebook. All Rights Reserved.

#pragma once

#include "fboss/agent/hw/sai/api/SaiApi.h"
#include "fboss/agent/hw/sai/api/SaiAttribute.h"
#include "fboss/agent/hw/sai/api/SaiAttributeDataTypes.h"
#include "fboss/agent/hw/sai/api/SaiVersion.h"
#include "fboss/agent/hw/sai/api/Types.h"

extern "C" {
#include <sai.h>
}

namespace facebook::fboss {

class TamApi;

struct SaiTamReport {
  static constexpr sai_object_type_t ObjectType = SAI_OBJECT_TYPE_TAM_REPORT;
  struct Attributes {
    using EnumType = sai_tam_report_attr_t;
    using Type = SaiAttribute<EnumType, SAI_TAM_REPORT_ATTR_TYPE, sai_int32_t>;
  };
  using AdapterKey = TamReportSaiId;
  using AdapterHostKey = Attributes::Type;
  using CreateAttributes = std::tuple<Attributes::Type>;
};

struct SaiTamEventActionTraits {
  static constexpr sai_object_type_t ObjectType =
      SAI_OBJECT_TYPE_TAM_EVENT_ACTION;
  struct Attributes {
    using EnumType = sai_tam_event_action_attr_t;
    using ReportType = SaiAttribute<
        EnumType,
        SAI_TAM_EVENT_ACTION_ATTR_REPORT_TYPE,
        sai_object_id_t>;
  };
  using AdapterKey = TamEventActionSaiId;
  using AdapterHostKey = Attributes::ReportType;
  using CreateAttributes = std::tuple<Attributes::ReportType>;
};

struct SaiTamEvent {
  static constexpr sai_object_type_t ObjectType = SAI_OBJECT_TYPE_TAM_EVENT;
  struct Attributes {
    using EnumType = sai_tam_event_attr_t;
    using Type = SaiAttribute<EnumType, SAI_TAM_EVENT_ATTR_TYPE, sai_int32_t>;
    using ActionList = SaiAttribute<
        EnumType,
        SAI_TAM_EVENT_ATTR_ACTION_LIST,
        std::vector<sai_object_id_t>>;
    using CollectorList = SaiAttribute<
        EnumType,
        SAI_TAM_EVENT_ATTR_COLLECTOR_LIST,
        std::vector<sai_object_id_t>>;
    using SwitchEventType = SaiAttribute<
        EnumType,
        SAI_TAM_EVENT_ATTR_SWITCH_EVENT_TYPE,
        std::vector<sai_int32_t>>;
  };
  using AdapterKey = TamEventSaiId;
  using AdapterHostKey = std::tuple<
      Attributes::Type,
      Attributes::ActionList,
      Attributes::CollectorList,
      Attributes::SwitchEventType>;
  using CreateAttributes = AdapterHostKey;
};

struct SaiTamTraits {
  static constexpr sai_object_type_t ObjectType = SAI_OBJECT_TYPE_TAM;
  struct Attributes {
    using EventObjectList = SaiAttribute<
        EnumType,
        SAI_TAM_ATTR_EVENT_OBJECTS_LIST,
        std::vector<sai_object_id_t>>;
    using TamBindPointList = SaiAttribute<
        EnumType,
        SAI_TAM_ATTR_TAM_BIND_POINT_TYPE_LIST,
        std::vector<sai_s32_list_t>>;
  };
  using AdapterKey = TamSaiId;
  using AdapterHostKey =
      std::tuple<Attributes::EventObjectList, Attributes::TamBindPointList>;
  using CreateAttributes = AdapterHostKey;
};

class TamApi : public SaiApi<TamApi> {
 public:
  static constexpr sai_api_t ApiType = SAI_API_PORT;
  TamApi() {
    sai_status_t status =
        sai_api_query(ApiType, reinterpret_cast<void**>(&api_));
    saiApiCheckError(status, ApiType, "Failed to query for port api");
  }

 private:
  // TAM
  sai_status_t _create(
      TamSaiId* id,
      sai_object_id_t switch_id,
      size_t count,
      sai_attribute_t* attr_list) const {
    return api_->create_tam(rawSaiId(id), switch_id, count, attr_list);
  }

  sai_status_t _remove(TamSaiId id) const {
    return api_->remove_tam(rawSaiId(id), switch_id, count, attr_list);
  }

  sai_status_t _getAttribute(TamSaiId id, sai_attribute_t* attr) const {
    return api_->get_tam_attribute(rawSaiId(id), 1, attr);
  }

  sai_status_t _setAttribute(TamSaiId id, const sai_attribute_t* attr) const {
    return api_->set_tam_attribute(rawSaiId(id), attr);
  }

  // TAM Event
  sai_status_t _create(
      TamEventSaiId* id,
      sai_object_id_t switch_id,
      size_t count,
      sai_attribute_t* attr_list) const {
    return api_->create_tam_event(rawSaiId(id), switch_id, count, attr_list);
  }

  sai_status_t _remove(TamEventSaiId id) const {
    return api_->remove_tam_event(rawSaiId(id), switch_id, count, attr_list);
  }

  sai_status_t _getAttribute(TamEventSaiId id, sai_attribute_t* attr) const {
    return api_->get_tam_event_attribute(rawSaiId(id), 1, attr);
  }

  sai_status_t _setAttribute(TamEventSaiId id, const sai_attribute_t* attr)
      const {
    return api_->set_tam_event_attribute(rawSaiId(id), attr);
  }

  // TAM Event Action
  sai_status_t _create(
      TamEventActionSaiId* id,
      sai_object_id_t switch_id,
      size_t count,
      sai_attribute_t* attr_list) const {
    return api_->create_tam_event_action(
        rawSaiId(id), switch_id, count, attr_list);
  }

  sai_status_t _remove(TamEventActionSaiId id) const {
    return api_->remove_tam_event_action(
        rawSaiId(id), switch_id, count, attr_list);
  }

  sai_status_t _getAttribute(TamEventActionSaiId id, sai_attribute_t* attr)
      const {
    return api_->get_tam_event_action_attribute(rawSaiId(id), 1, attr);
  }

  sai_status_t _setAttribute(
      TamEventActionSaiId id,
      const sai_attribute_t* attr) const {
    return api_->set_tam_event_action_attribute(rawSaiId(id), attr);
  }

  // TAM Report
  sai_status_t _create(
      TamReportSaiId* id,
      sai_object_id_t switch_id,
      size_t count,
      sai_attribute_t* attr_list) const {
    return api_->create_tam_report(rawSaiId(id), switch_id, count, attr_list);
  }

  sai_status_t _remove(TamReportSaiId id) const {
    return api_->remove_tam_report(rawSaiId(id));
  }

  sai_status_t _getAttribute(TamReportSaiId id, sai_attribute_t* attr) const {
    return api_->get_tam_report_attribute(rawSaiId(id), 1, attr);
  }

  sai_status_t _setAttribute(TamReportSaiId id, sai_attribute_t* attr) const {
    return api_->set_tam_report_attribute(rawSaiId(id), attr);
  }

  sai_tam_api_t* api_;
  friend class SaiApi<TamApi>;
};

} // namespace facebook::fboss
