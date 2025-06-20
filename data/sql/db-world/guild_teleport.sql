
CREATE TABLE `guild_teleport_locations` (
    `guild_id` INT PRIMARY KEY,
    `map` INT NOT NULL,
    `x` FLOAT NOT NULL,
    `y` FLOAT NOT NULL,
    `z` FLOAT NOT NULL,
    `o` FLOAT DEFAULT 0
);

INSERT INTO `acore_world`.`creature_template` (
  'entry', 'difficulty_entry_1', 'difficulty_entry_2', 'difficulty_entry_3',
  'KillCredit1', 'KillCredit2', 'name', 'subname', 'IconName', 'gossip_menu_id',
  'minlevel', 'maxlevel', 'exp', 'faction', 'npcflag',
  'speed_walk', 'speed_run', 'speed_swim', 'speed_flight',
  'detection_range', 'scale', 'rank', 'dmgschool', 'DamageModifier',
  'BaseAttackTime', 'RangeAttackTime', 'BaseVariance', 'RangeVariance',
  'unit_class', 'unit_flags', 'unit_flags2', 'dynamicflags', 'family',
  'trainer_type', 'trainer_spell', 'trainer_class', 'trainer_race',
  'type', 'type_flags', 'lootid', 'pickpocketloot', 'skinloot',
  'PetSpellDataId', 'VehicleId', 'mingold', 'maxgold',
  'AIName', 'MovementType', 'HoverHeight', 'HealthModifier', 'ManaModifier',
  'ArmorModifier', 'ExperienceModifier', 'RacialLeader', 'movementId',
  'RegenHealth', 'mechanic_immune_mask', 'spell_school_immune_mask',
  'flags_extra', 'ScriptName', 'VerifiedBuild'
) VALUES (
  70010, 0, 0, 0,
  0, 0, 'Guild Teleport NPC', '', NULL, 0,
  60, 60, 0, 35, 64,
  1.0, 2.0, 1.0, 1.0,
  18, 1.0, 0, 0, 1.0,
  2000, 2000, 0, 0,
  0, 0, 0, 0, 0,
  0, 0, 0, 0,
  7, 0, 0, 0, 0,
  0, 0, 0, 0,
  '', 0, 1.0, 1.0, 1.0,
  1.0, 0, 0, 0,
  0, 0, 0,
  0, 'GuildTeleportNPC', 1
);

REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (70001, 'GuildTeleportSpell');

INSERT IGNORE INTO `spellcasttimes_dbc` (`ID`, `Base`, `PerLevel`, `Minimum`) VALUES (9000, 10000, 0, 10000);
INSERT IGNORE INTO `spellcasttimes` (`ID`, `CastingTime`, `CastingTimePerLevel`, `MinimumCastingTime`) VALUES (9000, 10000, 0, 10000);

INSERT INTO `spell` (
  `ID`, `Category`, `Dispel`, `Mechanic`, `Attributes`, `AttributesEx`, `AttributesEx2`, `AttributesEx3`,
  `CastingTimeIndex`, `DurationIndex`, `RangeIndex`, `Speed`, `StackAmount`,
  `Effect1`, `EffectApplyAuraName1`, `EffectBasePoints1`, `EffectDieSides1`, `EffectRealPointsPerLevel1`, `EffectBaseDice1`, `EffectDiceSides1`, `EffectMechanic1`,
  `EffectImplicitTargetA1`, `EffectImplicitTargetB1`, `EffectRadiusIndex1`, `EffectChainTarget1`, `EffectPointsPerComboPoint1`,
  `Effect2`, `EffectApplyAuraName2`, `EffectBasePoints2`, `EffectDieSides2`, `EffectRealPointsPerLevel2`, `EffectBaseDice2`, `EffectDiceSides2`, `EffectMechanic2`,
  `EffectImplicitTargetA2`, `EffectImplicitTargetB2`, `EffectRadiusIndex2`, `EffectChainTarget2`, `EffectPointsPerComboPoint2`,
  `Effect3`, `EffectApplyAuraName3`, `EffectBasePoints3`, `EffectDieSides3`, `EffectRealPointsPerLevel3`, `EffectBaseDice3`, `EffectDiceSides3`, `EffectMechanic3`,
  `EffectImplicitTargetA3`, `EffectImplicitTargetB3`, `EffectRadiusIndex3`, `EffectChainTarget3`, `EffectPointsPerComboPoint3`,
  `SpellVisual1`, `SpellVisual2`
)
VALUES (
  70001, 0, 0, 0, 0, 0, 0, 0,
  9000, 1, 7, 1.5, 1,
  80, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,
  12345, 12345
);

INSERT INTO `spell_dbc` (
  `ID`, `Category`, `DispelType`, `Mechanic`, `Attributes`, `AttributesEx`, `AttributesEx2`, `AttributesEx3`,
  `CastingTimeIndex`, `DurationIndex`, `RangeIndex`, `Speed`, `StackAmount`,
  `Effect_1`, `EffectApplyAuraName_1`, `EffectBasePoints_1`, `EffectDieSides_1`, `EffectRealPointsPerLevel_1`, `EffectBaseDice_1`, `EffectDiceSides_1`, `EffectMechanic_1`,
  `ImplicitTargetA_1`, `ImplicitTargetB_1`, `EffectRadiusIndex_1`, `EffectChainTarget_1`, `EffectPointsPerComboPoint_1`,
  `Effect_2`, `EffectApplyAuraName_2`, `EffectBasePoints_2`, `EffectDieSides_2`, `EffectRealPointsPerLevel_2`, `EffectBaseDice_2`, `EffectDiceSides_2`, `EffectMechanic_2`,
  `ImplicitTargetA_2`, `ImplicitTargetB_2`, `EffectRadiusIndex_2`, `EffectChainTarget_2`, `EffectPointsPerComboPoint_2`,
  `Effect_3`, `EffectApplyAuraName_3`, `EffectBasePoints_3`, `EffectDieSides_3`, `EffectRealPointsPerLevel_3`, `EffectBaseDice_3`, `EffectDiceSides_3`, `EffectMechanic_3`,
  `ImplicitTargetA_3`, `ImplicitTargetB_3`, `EffectRadiusIndex_3`, `EffectChainTarget_3`, `EffectPointsPerComboPoint_3`,
  `SpellVisualID_1`, `SpellVisualID_2`
)
VALUES (
  70001, 0, 0, 0, 0, 0, 0, 0,
  9000, 1, 7, 1.5, 1,
  80, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,
  12345, 12345
);