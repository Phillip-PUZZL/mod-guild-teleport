
CREATE TABLE `guild_teleport_locations` (
    `guild_id` INT PRIMARY KEY,
    `map` INT NOT NULL,
    `x` FLOAT NOT NULL,
    `y` FLOAT NOT NULL,
    `z` FLOAT NOT NULL,
    `o` FLOAT DEFAULT 0
);

INSERT INTO `creature_template` (
    `entry`, `name`, `subname`, `minlevel`, `maxlevel`, `faction`, `npcflag`, `speed_walk`, `speed_run`,
    `scale`, `rank`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`,
    `type`, `type_flags`, `loot_id`, `pickpocket_loot_id`, `skinloot_id`, `resistance1`, `resistance2`,
    `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`,
    `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`,
    `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`,
    `mechanic_immune_mask`, `flags_extra`, `script_name`, `AIName`, `MovementType`, `InhabitType`,
    `HoverHeight`, `HealthModifier`, `ManaModifier`, `ArmorModifier`, `DamageModifier`,
    `RangedDamageModifier`, `ExperienceModifier`, `UnitMovementType`, `MovementId`, `RegenHealth`,
    `MechanicImmuneMask`, `FlagsExtra`, `ScriptName`, `AIName`
) VALUES (
    70010, 'Guild Teleport NPC', '', 60, 60, 35, 64, 1.0, 2.0,
    1.0, 0, 0, 0, 0, 0, 0,
    7, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    2000, 2000, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 'GuildTeleportNPC', '', 0, 0,
    0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 0, 0, 0,
    0, 0, 'GuildTeleportNPC', ''
);

REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (70001, 'GuildTeleportSpell');

INSERT IGNORE INTO `spell_template` (`Id`) VALUES (70001);

INSERT IGNORE INTO `spellcasttimes` (`ID`, `Base`, `PerLevel`, `Minimum`) VALUES (9000, 10000, 0, 10000);

REPLACE INTO `custom_spell_template` (
    `entry`,
    `Category`,
    `Dispel`,
    `Mechanic`,
    `Attributes`,
    `AttributesEx`,
    `AttributesEx2`,
    `AttributesEx3`,
    `CastingTimeIndex`,
    `DurationIndex`,
    `RangeIndex`,
    `Speed`,
    `StackAmount`,
    `Effect[0]`,
    `EffectApplyAuraName[0]`,
    `EffectBasePoints[0]`,
    `EffectDieSides[0]`,
    `EffectRealPointsPerLevel[0]`,
    `EffectBaseDice[0]`,
    `EffectDiceSides[0]`,
    `EffectMechanic[0]`,
    `EffectImplicitTargetA[0]`,
    `EffectImplicitTargetB[0]`,
    `EffectRadiusIndex[0]`,
    `EffectChainTarget[0]`,
    `EffectPointsPerComboPoint[0]`,
    `Effect[1]`,
    `EffectApplyAuraName[1]`,
    `EffectBasePoints[1]`,
    `EffectDieSides[1]`,
    `EffectRealPointsPerLevel[1]`,
    `EffectBaseDice[1]`,
    `EffectDiceSides[1]`,
    `EffectMechanic[1]`,
    `EffectImplicitTargetA[1]`,
    `EffectImplicitTargetB[1]`,
    `EffectRadiusIndex[1]`,
    `EffectChainTarget[1]`,
    `EffectPointsPerComboPoint[1]`,
    `Effect[2]`,
    `EffectApplyAuraName[2]`,
    `EffectBasePoints[2]`,
    `EffectDieSides[2]`,
    `EffectRealPointsPerLevel[2]`,
    `EffectBaseDice[2]`,
    `EffectDiceSides[2]`,
    `EffectMechanic[2]`,
    `EffectImplicitTargetA[2]`,
    `EffectImplicitTargetB[2]`,
    `EffectRadiusIndex[2]`,
    `EffectChainTarget[2]`,
    `EffectPointsPerComboPoint[2]`,
    `SpellVisual[0]`,
    `SpellVisual[1]`
)
VALUES (
    70001, 0, 0, 0, 0, 0, 0, 0,
    9000, -- Cast time (points to 10000 ms in spellcasttimes)
    1,    -- Instant duration
    7,    -- Range: SPELL_RANGE_TARGET (7)
    1.5,  -- Speed
    1,    -- Stack amount
    80,   -- Effect 0: SPELL_EFFECT_TELEPORT_UNITS
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    12345, 12345 -- Optional spell visuals
);