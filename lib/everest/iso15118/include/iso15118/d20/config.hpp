// SPDX-License-Identifier: Apache-2.0
// Copyright 2026 Pionix GmbH and Contributors to EVerest
#pragma once

#include <cstdint>
#include <ctime>
#include <map>
#include <optional>
#include <vector>

#include <iso15118/d20/der_functions.hpp>
#include <iso15118/d20/limits.hpp>
#include <iso15118/message/common_types.hpp>

// EvseSetupConfig / SessionConfig (the universal SECC-side session configuration consumed by the
// ISO 15118-20, ISO 15118-2 and DIN SPEC 70121 SECC engines) live in the protocol-neutral
// iso15118::session namespace (see include/iso15118/session/config.hpp). The structs below remain in
// d20 as they are expressed with the -20 RationalNumber datatype / IEC 15118-20 DER control types, and
// are referenced qualified (d20::...) from the session-side structs.
namespace iso15118::d20 {

struct ControlMobilityNeedsModes {
    message_20::datatypes::ControlMode control_mode;
    message_20::datatypes::MobilityNeedsMode mobility_mode;
};

struct AcSetupConfig {
    uint32_t voltage;
    std::vector<message_20::datatypes::AcConnector> connectors;
};

struct BptSetupConfig {
    message_20::datatypes::BptChannel bpt_channel;
    message_20::datatypes::GeneratorMode generator_mode;
    std::optional<message_20::datatypes::GridCodeIslandingDetectionMethod> grid_code_detection_method;
};

struct DerIecSetupConfig {
    std::map<iec::DERControlName, iec::DERControlFunction> supported_der_control_functions;
    iec::OperatingMode operating_mode;
    iec::GridConnectionMode grid_connection_mode;
};

/// \brief Complete but deliberately inert default SAE grid code configuration.
///
/// Every enable and permit_service is false, so it does nothing. Values stay schema conformant, and every
/// mandatory curve carries the two data points the schema requires as a minimum. This is not a real grid
/// code: a deployment needing actual grid-code behavior must supply its own configuration.
///
/// The two EnterService voltage bands and the VoltVar reference voltage are volts, not percentages
/// (AMD1 Table 1), so they are derived from nominal_voltage_v.
sae::DERControl get_default_sae_der_control(float nominal_voltage_v);

struct DerSaeSetupConfig {
    explicit DerSaeSetupConfig(sae::DERControl der_control_, sae::RequiredDEROperatingMode op_mode,
                               sae::GridConnectionMode conn_mode) :
        der_control(std::move(der_control_)),
        required_der_operating_mode(op_mode),
        grid_connection_mode(conn_mode),
        der_control_update_time(static_cast<std::uint64_t>(std::time(nullptr))) {
    }

    sae::DERControl der_control{};
    sae::RequiredDEROperatingMode required_der_operating_mode{sae::RequiredDEROperatingMode::GridFollowing};
    sae::GridConnectionMode grid_connection_mode{sae::GridConnectionMode::GridConnected};
    std::uint64_t der_control_update_time{0}; // SECC time
    // Producer-owned change counter; der_control_update_time is the wire UpdateTime only (ADR-0027).
    std::uint32_t revision{0};
};

/// Inert default grid code with GridFollowing/GridConnected; not a real grid code.
DerSaeSetupConfig make_inert_default_sae_setup_config(float nominal_voltage_v);

} // namespace iso15118::d20
