//extern int asm_sev_log;

template <typename RAND_CLASS>
int generate_item(unsigned int id, item_data ** item, unsigned int& size, RAND_CLASS cls ,element_data::GEN_ADDON_MODE normal_addon,const void * tag, unsigned int tag_size, int class_pos)
{
	int ret = -1;
	DATA_TYPE datatype = get_data_type(id, ID_SPACE_ESSENCE);
	switch(datatype)
	{
	case DT_WEAPON_ESSENCE:
		ret = generate_weapon(id, ID_SPACE_ESSENCE, (char **)item, size, cls,normal_addon,tag,tag_size);
		break;

	case DT_ARMOR_ESSENCE:
		ret = generate_armor(id, ID_SPACE_ESSENCE, (char **)item, size, cls,normal_addon,tag,tag_size);
		break;

	case DT_NEW_ARMOR_ESSENCE:
		ret = generate_new_armor(id, ID_SPACE_ESSENCE, (char **)item, size, cls,normal_addon,tag,tag_size);
		break;

	case DT_DECORATION_ESSENCE:
		ret = generate_decoration(id, ID_SPACE_ESSENCE, (char **)item, size, cls,normal_addon,tag,tag_size);
		break;

	case DT_PROJECTILE_ESSENCE:
		//ret = generate_projectile(id, ID_SPACE_ESSENCE, (char **)item, size, cls);
		//�������еļ�֧�������?
		ret = duplicate_static_item(id, (char **)item, size);
		break;
	
	case DT_QUIVER_ESSENCE:
		ret = generate_quiver(id, ID_SPACE_ESSENCE, (char **)item, size, cls);
		break;

	case DT_STONE_ESSENCE:
		//�������е�stone�������?
		ret = duplicate_static_item(id, (char **)item, size);
		break;
		
	case DT_TASKDICE_ESSENCE:
		ret = generate_taskdice(id, ID_SPACE_ESSENCE, (char **)item, size, cls);
		break;

	case DT_FLYSWORD_ESSENCE:
		ret = generate_flysword(id, ID_SPACE_ESSENCE, (char **)item, size, cls,tag,tag_size);
		break;

	case DT_FASHION_ESSENCE:
		ret = generate_fashion_item(id, ID_SPACE_ESSENCE, (char **)item, size,cls,tag,tag_size);
		break;

	case DT_GOBLIN_ESSENCE:
		ret = generate_elf(id, ID_SPACE_ESSENCE, (char **)item, size);
		break;

	case DT_GOBLIN_EQUIP_ESSENCE:
		ret = generate_elf_equip(id, ID_SPACE_ESSENCE, (char **)item, size);
		break;

	case DT_GOBLIN_EXPPILL_ESSENCE:
		ret = generate_elf_exppill(id, ID_SPACE_ESSENCE, (char **)item, size);
		break;

	case DT_SELL_CERTIFICATE_ESSENCE:
		ret = generate_stallcard(id, ID_SPACE_ESSENCE, (char **)item, size);
		break;

	case DT_TARGET_ITEM_ESSENCE:
		ret = generate_skilltrigger2(id,ID_SPACE_ESSENCE,(char **)item, size);
		break;

	case DT_LOOK_INFO_ESSENCE:
		ret = generate_queryotherproperty(id,ID_SPACE_ESSENCE,(char **)item, size);
		break;

	case DT_INC_SKILL_ABILITY_ESSENCE:
		ret = generate_incskillability(id,ID_SPACE_ESSENCE,(char **)item,size);
		break;
	
	case DT_WEDDING_BOOKCARD_ESSENCE:
		ret = generate_wedding_bookcard(id,ID_SPACE_ESSENCE,(char **)item,size);
		break;
	
	case DT_WEDDING_INVITECARD_ESSENCE:
		ret = generate_wedding_invitecard(id,ID_SPACE_ESSENCE,(char **)item,size);
		break;
	
	case DT_SHARPENER_ESSENCE:
		ret = generate_sharpener(id,ID_SPACE_ESSENCE,(char **)item,size);
		break;
		
	case DT_POKER_ESSENCE:	
		ret = generate_generalcard(id,ID_SPACE_ESSENCE,(char **)item,size,cls);
		break;
	
	case DT_ASTROLABE_ESSENCE: 	
		ret = generate_astrolabe(id,ID_SPACE_ESSENCE,(char **)item,size);
		break;

	case DT_WINGMANWING_ESSENCE:
	case DT_MEDICINE_ESSENCE:
	case DT_MATERIAL_ESSENCE:
	case DT_DYE_TICKET_ESSENCE:
	case DT_ELEMENT_ESSENCE:
	case DT_SKILLTOME_ESSENCE:
	case DT_TOWNSCROLL_ESSENCE:
	case DT_DAMAGERUNE_ESSENCE:
	case DT_ARMORRUNE_ESSENCE:
	case DT_TASKMATTER_ESSENCE:
	case DT_TOSSMATTER_ESSENCE:
	case DT_TASKNORMALMATTER_ESSENCE:
	case DT_FACEPILL_ESSENCE:
	case DT_FACETICKET_ESSENCE:
	case DT_GM_GENERATOR_ESSENCE:
	case DT_PET_EGG_ESSENCE:
	case DT_PET_FOOD_ESSENCE:
	case DT_FIREWORKS_ESSENCE:
	case DT_WAR_TANKCALLIN_ESSENCE:
	case DT_SKILLMATTER_ESSENCE:
	case DT_BIBLE_ESSENCE:
	case DT_SPEAKER_ESSENCE:
	case DT_AUTOMP_ESSENCE:
	case DT_AUTOHP_ESSENCE:
	case DT_DOUBLE_EXP_ESSENCE:
	case DT_TRANSMITSCROLL_ESSENCE:
	case DT_FACTION_MATERIAL_ESSENCE:
	case DT_CONGREGATE_ESSENCE:
	case DT_FORCE_TOKEN_ESSENCE:
	case DT_DYNSKILLEQUIP_ESSENCE:
	case DT_MONEY_CONVERTIBLE_ESSENCE:
	case DT_POKER_DICE_ESSENCE:
	case DT_MONSTER_SPIRIT_ESSENCE:
	case DT_SHOP_TOKEN_ESSENCE:
	case DT_UNIVERSAL_TOKEN_ESSENCE:
	case DT_ASTROLABE_RANDOM_ADDON_ESSENCE:
	case DT_ASTROLABE_INC_INNER_POINT_VALUE_ESSENCE:
	case DT_ASTROLABE_INC_EXP_ESSENCE:
	case DT_ITEM_PACKAGE_BY_PROFESSION_ESSENCE:
	case DT_FIREWORKS2_ESSENCE:
	case DT_FIX_POSITION_TRANSMIT_ESSENCE:
		ret = duplicate_static_item(id, (char **)item, size);
		break;

	case DT_EASY_PRODUCE_ITEM_ESSENCE:
		ret = generate_easy_produce_item(id, ID_SPACE_ESSENCE, (char **)item, size, cls);
		break;

	case DT_UNLOCK_RUNE_SLOT_ITEM_ESSENCE:
		ret = generate_rune_slot_item_essence(id, ID_SPACE_ESSENCE, (char **)item, size, cls);
		break;

	case DT_RUNE_ITEM_ESSENCE:
		ret = generate_rune_item_essence(id, ID_SPACE_ESSENCE, (char **)item, size, cls);
		break;

	case DT_FASHION_NEW_ESSENCE:
		ret = generate_fashion_new_item(id, ID_SPACE_ESSENCE, (char **)item, size,cls,tag,tag_size);
		break;

	case DT_FIX_MONSTER_ITEM_ESSENCE:
		ret = generate_fix_monster_item(id, ID_SPACE_ESSENCE, (char **)item, size, cls);
		break;
		
	case DT_BIBLE_REFINE_TICKET_ESSENCE:
		ret = generate_bible_refine_ticket_item(id, ID_SPACE_ESSENCE, (char **)item, size, cls);
		break;
	
	case DT_REFINE_TICKET_ESSENCE:
		ret = generate_refine_ticket(id, ID_SPACE_ESSENCE, (char **)item, size, cls);
		break;

	case DT_RED_BOOK_UPGRADE_ITEM:
		ret = generate_red_book_upgrade_item(id, ID_SPACE_ESSENCE, (char **)item, size, cls);
		break;

	case DT_ITEM_USED_FOR_AREA_ESSENCE:
		ret = generate_used_for_area_item(id, ID_SPACE_ESSENCE, (char **)item, size, cls);
		break;

	case DT_CAPTURE_ITEM_ESSENCE:
		ret = generate_capture_item(id, ID_SPACE_ESSENCE, (char **)item, size, cls);
		break;

	case DT_QIHUN_ESSENCE:
		ret = generate_qihun_item(id, ID_SPACE_ESSENCE, (char **)item, size, cls,normal_addon,tag,tag_size,class_pos);
		break;

	case DT_QILING_ESSENCE:
		ret = generate_qiling_item(id, ID_SPACE_ESSENCE, (char **)item, size, cls,normal_addon,tag,tag_size,0);
		break;

	case DT_QIHUN_COVER_ESSENCE:
		ret = generate_qihun_cover_item(id, ID_SPACE_ESSENCE, (char **)item, size, cls);
		break;

	case DT_SLIDE_SKILL_ESSENCE:
		ret = generate_slide_skill_item(id, ID_SPACE_ESSENCE, (char **)item, size, cls);
		break;

	case DT_USE_FOR_SELF_ITEM_ESSENCE:
		ret = generate_use_for_self_item(id, ID_SPACE_ESSENCE, (char **)item, size, cls);
		break;

	case DT_CAMP_TOKEN_ESSENCE:
		ret = generate_camp_token_item(id, ID_SPACE_ESSENCE, (char **)item, size, cls);
		break;

	case DT_CAMP_TOKEN_UPGRADE_ESSENCE:
		ret = generate_camp_token_upgrade_item(id, ID_SPACE_ESSENCE, (char **)item, size, cls);
		break;
	
	case DT_CAMP_TOKEN_ROLLBACK_ESSENCE:
		ret = generate_camp_token_rollback_item(id, ID_SPACE_ESSENCE, (char **)item, size, cls);
		break;

	case DT_CAMP_TOKEN_PROB_ADJUST_ESSENCE:
		ret = generate_camp_token_prob_adjust_item(id, ID_SPACE_ESSENCE, (char **)item, size, cls);
		break;

	case DT_ILLUSTRATED_FASHION_ESSENCE:
		ret = generate_illustrated_fashion_item(id, ID_SPACE_ESSENCE, (char **)item, size, cls);
		break;

	case DT_ILLUSTRATED_WING_EGG_ESSENCE:
		ret = generate_illustrated_wing_egg_item(id, ID_SPACE_ESSENCE, (char **)item, size, cls);
		break;

	case DT_ILLUSTRATED_PET_EGG_ESSENCE:
		ret = generate_illustrated_pet_egg_item(id, ID_SPACE_ESSENCE, (char **)item, size, cls);
		break;

	case DT_FAST_PRODUCE_ITEM_ESSENCE:
		ret = generate_fast_produce_item(id, ID_SPACE_ESSENCE, (char **)item, size, cls);
		break;

	case DT_KID_DEBRIS_ESSENCE:
		ret = generate_kid_debris_item(id, ID_SPACE_ESSENCE, (char **)item, size, cls);
		break;

	case DT_KID_DEBRIS_GENERATOR_ESSENCE:
		ret = generate_kid_debris_generator_item(id, ID_SPACE_ESSENCE, (char **)item, size, cls);
		break;

	case DT_MATERIAL_REFINE_ESSENCE:
		ret = generate_material_refine_item(id, ID_SPACE_ESSENCE, (char **)item, size, cls);
		break;

	case DT_MATERIAL_REFINE_TICKET_ESSENCE:
		ret = generate_material_refine_ticket_item(id, ID_SPACE_ESSENCE, (char **)item, size, cls);
		break;

	case DT_UNIONSCROLL_ESSENCE:
	case DT_REVIVESCROLL_ESSENCE:
		ret = duplicate_static_item(id, (char **)item, size);
		break;

	case DT_MONSTER_ESSENCE:
	case DT_NPC_ESSENCE:
	default:
//		assert(false);
		*item = NULL;
		size = 0;
		return -1;
	}
	return ret;
}


template <typename RAND_CLASS>
inline void generate_template_addon(DATA_TYPE dt, float unique_prob, char * unique, /*float hide_prob,char * hide,*/ char * produce, char * drop, char * addon_buf, unsigned int &addon_num,unsigned &addon_size, RAND_CLASS cls,element_data::GEN_ADDON_MODE normal_addon,int * sa_list)
{
	if(normal_addon == element_data::ADDON_LIST_DROP)
	{
		unsigned int un = 0;
		unsigned int hi = 0;
		ASSERT(addon_size == 0);
		if(element_data::Rand(0.f,1.f,cls,element_data::LOWER_TREND) < unique_prob)
		{
			un = 1;
			addon_size = generate_equipment_addon_buffer(dt, unique, 16, addon_buf,un);
			addon_num -= un;
		}
		addon_size += generate_equipment_addon_buffer(dt, drop, 32, addon_buf + addon_size,addon_num);
		addon_num += un;

	}
	else if(normal_addon == element_data::ADDON_LIST_PRODUCE)
	{
		unsigned int un = 0;
		unsigned int hi = 0;
		ASSERT(addon_size == 0);
		if(element_data::Rand(0.f,1.f,cls,element_data::LOWER_TREND) < unique_prob)
		{
			un = 1;
			addon_size = generate_equipment_addon_buffer(dt, unique, 16, addon_buf,un);
			addon_num -= un;
		}
		addon_size += generate_equipment_addon_buffer(dt, produce, 32, addon_buf+addon_size,addon_num);
		addon_num += un;

	}
	else  if (normal_addon == element_data::ADDON_LIST_SPEC)
	{
		addon_size = generate_spec_addon_buffer(dt,addon_buf,ELEMENTDATAMAN_MAX_NUM_ADDONS,addon_num,sa_list);
	}
	else
	{
		addon_size = 0;
		addon_num = 0;
	}
}

template <typename RAND_CLASS>
inline void generate_template_addon_1(DATA_TYPE dt, float unique_prob, char * unique, float hide_prob,char * hide, int * addon, char * addon_buf, unsigned int &addon_num,unsigned &addon_size, RAND_CLASS cls,element_data::GEN_ADDON_MODE normal_addon,int * sa_list)
{
	if(normal_addon == element_data::ADDON_LIST_DROP || normal_addon == element_data::ADDON_LIST_PRODUCE)
	{
		unsigned int un = 0;
		unsigned int hi = 0;
		ASSERT(addon_size == 0);
		
		//if(asm_sev_log)
		//printf("HIDE_ADDON::GENERATE: hide_prob = %f , hide[1] = %d \n", hide_prob , (int)&hide[1] );
		
		if(element_data::Rand(0.f,1.f,cls,element_data::LOWER_TREND) < unique_prob)
		{
			un = 1;
			addon_size += generate_equipment_addon_buffer(dt, unique, 16, addon_buf,un);
		}
		
		if(element_data::Rand(0.f,1.f,cls,element_data::LOWER_TREND) < hide_prob)
		{
			hi = 1;
			addon_size += generate_equipment_addon_buffer(dt, hide, 16, addon_buf + addon_size,hi);
		}			

		addon_size += generate_equipment_addon_buffer_2(dt, addon, sizeof(int)+sizeof(float) , 32, addon_buf + addon_size,addon_num);
		addon_num += un + hi;
	}
	else  if (normal_addon == element_data::ADDON_LIST_SPEC)
	{
		addon_size += generate_spec_addon_buffer(dt,addon_buf,ELEMENTDATAMAN_MAX_NUM_ADDONS,addon_num,sa_list);
	}
	else
	{
		addon_size = 0;
		addon_num = 0;
	}
}

template <typename RAND_CLASS>
int generate_weapon(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls,element_data::GEN_ADDON_MODE normal_addon ,const void * tag,unsigned int tag_size,int * sa_list = NULL)
{
	DATA_TYPE datatype;
	unsigned int i=0;
	WEAPON_ESSENCE * ess = (WEAPON_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if( ess == NULL || datatype != DT_WEAPON_ESSENCE)	return -1;

	//$$$$$$$$$$$ ���ڼ�����Ʒû�ж��������Բ�������
	if(ess->id_sub_type == 300 || ess->id_sub_type == 293 || ess->id_sub_type == 76 || ess->id_sub_type == 291)
	{
		return -1;
	}
		
	size = sizeof(item_data) + sizeof(_item_content) + sizeof(_weapon_essence);
	
	// ����׶�?
	unsigned int hole_num = 0;
	if(normal_addon == element_data::ADDON_LIST_DROP)
	{
		hole_num = element_data::RandSelect(&(ess->drop_probability_socket0), sizeof(float), 3, cls,element_data::LOWER_TREND);		//�׶�����Ŀ
	}
	else //if(normal_addon == element_data::ADDON_LIST_PRODUCE || normal_addon == element_data::ADDON_LIST_SPEC)
	{
		hole_num = element_data::RandSelect(&(ess->make_probability_socket0), sizeof(float), 3, cls,element_data::LOWER_TREND);		//�׶�����Ŀ
	}
	size += hole_num*sizeof(int);	// size ����hole_num������type

	//����tag size
	ASSERT(tag_size >= sizeof(short));
	size += tag_size;

	
	// ���addons
	char addon_buf[ELEMENTDATAMAN_MAX_NUM_ADDONS*sizeof(_addon)];
	unsigned int addon_num = element_data::RandSelect(&(ess->probability_addon_num0),sizeof(float), 7 /*6*/, cls,element_data::LOWER_TREND);	//���Ա���Ŀ����Ŀ
	
	unsigned int addon_size = 0;
	
	switch (ess->fixed_props)
    {
    case 0:
		if(addon_num || normal_addon == element_data::ADDON_LIST_SPEC)
		{
			generate_template_addon(DT_WEAPON_ESSENCE,ess->probability_unique,
				(char*)ess->uniques,(char*)ess->rands,(char*)ess->addons,
				addon_buf,addon_num,addon_size,cls,normal_addon,sa_list);
		}
		else
		{
			if (normal_addon == element_data::ADDON_LIST_SPEC)
			{
				addon_size = generate_spec_addon_buffer(DT_WEAPON_ESSENCE,addon_buf,ELEMENTDATAMAN_MAX_NUM_ADDONS,addon_num,sa_list);
			}
		}
		break;
	case 4:
		if(addon_num || normal_addon == element_data::ADDON_LIST_SPEC)
		{
			generate_template_addon_1(DT_WEAPON_ESSENCE,ess->probability_unique,
				(char*)ess->uniques,ess->probability_hide,(char*)ess->hides,(int*)&(ess->addons[0]),
				addon_buf,addon_num,addon_size,cls,normal_addon,sa_list);
		}
		else
		{
			if (normal_addon == element_data::ADDON_LIST_SPEC)
			{
				addon_size = generate_spec_addon_buffer(DT_WEAPON_ESSENCE,addon_buf,ELEMENTDATAMAN_MAX_NUM_ADDONS,addon_num,sa_list);
			}
		}	
		break;
    default:
		addon_size = generate_equipment_addon_buffer_2(DT_WEAPON_ESSENCE, (int*)&(ess->addons[0]), sizeof(int)+sizeof(float),32, addon_buf,addon_num);
        break;
    }
	
	size += addon_size;
	
	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = *data;
	
	*(unsigned int*)buf = id;				buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;						buf += sizeof(unsigned int);			//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);			//��Ʒ�Ķѵ�����
	*(int*)buf = ELEMENTDATAMAN_EQUIP_MASK_WEAPON|(addon_num?0x40000000:0);			buf += sizeof(int);				//��Ʒ�Ŀ�װ����־���̶�ֵ����
	*(int*)buf = ess->proc_type;			buf += sizeof(int);				//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_WEAPON_ESSENCE;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID		
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;				buf += sizeof(int);				//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char *)(*data)+size-buf;
	*item_content = buf;
	
	// prerequisition
	char * require_ptr = buf;
	*(short*)buf = ess->require_level;		buf += sizeof(short);				//prerequisition level
	*(short*)buf = ess->character_combo_id&0xFFFF;	buf += sizeof(short);				//prerequisition race
	*(short*)buf = ess->require_strength;		buf += sizeof(short);				//prerequisition strength
	*(short*)buf = ess->require_tili;		buf += sizeof(short);				//prerequisition val
	*(short*)buf = ess->require_agility;		buf += sizeof(short);				//prerequisition agility
	*(short*)buf = ess->require_energy;		buf += sizeof(short);				//prerequisition energy
	

	int temp2 = element_data::RandNormal(ess->durability_min, ess->durability_max, cls,element_data::LOWER_TREND);
	int temp;
	if(normal_addon != element_data::ADDON_LIST_DROP || ess->proc_type & 0x1000)
	{
		temp = temp2;
	}
	else
	{
		temp = element_data::RandNormal(ess->durability_drop_min, ess->durability_drop_max, cls,element_data::UPPER_TREND);
		if(temp > temp2) temp = temp2;
	}

	*(int*)buf = temp;						buf += sizeof(int);				//prerequisition durability
	*(int*)buf = temp2;						buf += sizeof(int);				//prerequisition max_durability

	//���������߱�ǩע���ǩ���ڱ����С�ͱ�������֮��
	*(short*)buf = sizeof(_weapon_essence);	buf += sizeof(short);				//װ�������С���ֽڣ�?
	memcpy(buf,tag,tag_size);
	buf += tag_size;
	
	//essence					//char ����[];			//ÿ�ֲ�ͬװ���ı���ṹ���?
	char * essence_ptr = buf;
	switch(ess->short_range_mode)
	{
		default:
		case 0: *(short*)buf = 1; break;	//Զ��
		case 1: *(short*)buf = 0; break;	//����
		case 2: *(short*)buf = 2; break;	//�̿ͽ���
		case 3: *(short*)buf = 3; break;	//SHOOTER 162
	}
	buf += sizeof(short);		//�������? ��Ӧģ����Ľ���Զ�̱��? 
	*(short*)buf = 0;				buf += sizeof(short);		//���ֵ����ʹ��?
	*(int*)buf = ess->id_major_type;		buf += sizeof(int);				//�������� ��Ӧģ����Ĵ���? ���絶�� ������	
	*(int*)buf = ess->level;				buf += sizeof(int);				//�������� ĳЩ������Ҫ��������
	*(int*)buf = ess->require_projectile;	buf += sizeof(int);				//��Ҫ��ҩ������
	*(int*)buf = ess->damage_low;			buf += sizeof(int);				//����������С��ֵ	
	*(int*)buf = element_data::RandNormal(ess->damage_high_min, ess->damage_high_max, cls,element_data::LOWER_TREND);		buf += sizeof(int);				//������������ֵ
	*(int*)buf = ess->magic_damage_low;		buf += sizeof(int);				//ħ����������?
	*(int*)buf = element_data::RandNormal(ess->magic_damage_high_min, ess->magic_damage_high_max, cls,element_data::LOWER_TREND);	buf += sizeof(int);		//ħ������
	
	WEAPON_SUB_TYPE * subtype = (WEAPON_SUB_TYPE * )get_data_ptr(ess->id_sub_type, ID_SPACE_ESSENCE, datatype);
	if(subtype == NULL || datatype != DT_WEAPON_SUB_TYPE)
	{
		//		assert(false);
		*(int*)buf = 0;		buf += sizeof(int);	
	}
	else
	{
		unsigned int index = element_data::RandSelect(&(subtype->probability_fastest), sizeof(float), 5, cls,element_data::MIDDLE_TREND);
		*(int*)buf = (int)(subtype->attack_speed*20.f + 0.1f) + (index - 2);		buf += sizeof(int);		//ģ���е���0.05����?
	}
	*(float*)buf = ess->attack_range;					buf += sizeof(float);
	*(float*)buf = subtype->attack_short_range;			buf += sizeof(float);			
	
	*(short*)buf = hole_num;		buf += sizeof(short);				//�׶�����Ŀ
	*(short*)buf = 0;			buf += sizeof(short);				//�׶���������������
	for(i=0; i<hole_num; i++)
	{
		*(int*)buf = 0;			buf += sizeof(int);					//�׶���Ƕ��������ͣ�û�ж���?0
	}
	
	// addons
	*(int*)buf = addon_num;		buf += sizeof(int);
	char * addon_sld = addon_buf;
	for(i=0; i<addon_num; i++)
	{
		int id = *(int*)addon_sld;
		//printf("generate_weapon: id = %d , i = %d , addon_num = %d \n", id , i , addon_num);
		addon_update_ess_data(*(addon_data*)addon_sld, essence_ptr, sizeof(_weapon_essence), (prerequisition*)require_ptr);
		addon_sld += ((id & 0x6000)>>13)*sizeof(int)+sizeof(int);
	}

	update_require_data((prerequisition*)require_ptr);
	memcpy(buf, addon_buf, addon_size);	
	set_to_classid(DT_WEAPON_ESSENCE, (item_data*)(*data), -1);
	
	return 0;
}

template <typename RAND_CLASS>
void generate_magic_defense(int res[5], int * res_list , RAND_CLASS cls,bool fixed = false)
{
	                          // 5   4      3     2     1
	static float count_prop[] = {0.35f,0.25f,0.20f,0.15f,0.05f,0.051f};
	static float md_adjust[] = {1.0f,1.1f,1.3f,1.6f,2.0f};
	int RE_num = 0;
	if(!fixed ) RE_num = element_data::RandSelect(count_prop,sizeof(float), 6, cls,element_data::LOWER_TREND);
	if(RE_num == 5) return;  //����������
	int md[5]= {0,1,2,3,4};
	int i;
	for(i = 0; i < RE_num; i ++)
	{
		int r = abase::Rand(i,4);
		
		int t = md[i];
		md[i] = md[r];
		md[r] = t;
	}

	//ʣ�µ����ݾ��������������?
	float adj = md_adjust[RE_num];
	for(i = 0; i < 5 - RE_num; i++)
	{
		int index = md[i];
		int low = res_list[index *2];
		int high= res_list[index *2+1];
		res[index] = (int)(element_data::RandNormal(low, high, cls,element_data::LOWER_TREND) * adj);
	}
}


template <typename RAND_CLASS>
int generate_armor(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls,element_data::GEN_ADDON_MODE normal_addon,const void * tag,unsigned int tag_size,int * sa_list = NULL)
{
	DATA_TYPE datatype;
	unsigned int i=0;
	ARMOR_ESSENCE * ess = (ARMOR_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_ARMOR_ESSENCE)	return -1;

	size = sizeof(item_data) + sizeof(_item_content) + sizeof(_armor_essence);
	// ����׶�?
	unsigned int hole_num = 0;
	if(normal_addon == element_data::ADDON_LIST_DROP)
	{
		hole_num = element_data::RandSelect(&(ess->drop_probability_socket0), sizeof(float), 5, cls,element_data::LOWER_TREND);		//�׶�����Ŀ
	}
	else //if(normal_addon == element_data::ADDON_LIST_PRODUCE || normal_addon == element_data::ADDON_LIST_SPEC)
	{
		hole_num = element_data::RandSelect(&(ess->make_probability_socket0), sizeof(float), 5, cls,element_data::LOWER_TREND);		//�׶�����Ŀ
	}
	size += hole_num*sizeof(int);	// size ����hole_num������type

	//����tag size
	ASSERT(tag_size >= sizeof(short));
	size += tag_size;

	
	// ���addons
	char addon_buf[ELEMENTDATAMAN_MAX_NUM_ADDONS*sizeof(_addon)];
	unsigned int addon_num = element_data::RandSelect(&(ess->probability_addon_num0),sizeof(float), 5, cls,element_data::LOWER_TREND);	//���Ա���Ŀ����Ŀ

	unsigned int addon_size = 0;
	if(ess->fixed_props)
	{
		addon_size = generate_equipment_addon_buffer_2(DT_ARMOR_ESSENCE, (int*)&(ess->addons[0]), sizeof(int)+sizeof(float), 32, addon_buf,addon_num);
	}
	else
	{
		if(normal_addon == element_data::ADDON_LIST_DROP)
			addon_size = generate_equipment_addon_buffer(DT_ARMOR_ESSENCE, (char*)&(ess->addons[0]), 32, addon_buf,addon_num);
		else if(normal_addon == element_data::ADDON_LIST_PRODUCE)
			addon_size = generate_equipment_addon_buffer(DT_ARMOR_ESSENCE, (char*)&(ess->rands[0]), 32, addon_buf,addon_num);
		else  if (normal_addon == element_data::ADDON_LIST_SPEC)
		{
			addon_size = generate_spec_addon_buffer(DT_ARMOR_ESSENCE,addon_buf,ELEMENTDATAMAN_MAX_NUM_ADDONS,addon_num,sa_list);
		}
		else
		{
			addon_size = 0;
			addon_num = 0;
		}
	}
	size += addon_size;
		
	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);


	*(unsigned int*)buf = id;				buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;						buf += sizeof(unsigned int);			//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);			//��Ʒ�Ķѵ�����
	ARMOR_SUB_TYPE* sub_type = (ARMOR_SUB_TYPE*)get_data_ptr(ess->id_sub_type, ID_SPACE_ESSENCE, datatype);
	if(sub_type == NULL || datatype != DT_ARMOR_SUB_TYPE)				// error
		return -1;
	*(int*)buf = sub_type->equip_mask|(addon_num?ELEMENTDATAMAN_EQUIP_MASK_HAS_ADDON:0);		buf += sizeof(int);				//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);				//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_ARMOR_ESSENCE;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;				buf += sizeof(int);				//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	// prerequisition
	char * require_ptr = buf;
	*(short*)buf = ess->require_level;		buf += sizeof(short);				//prerequisition level
	*(short*)buf = ess->character_combo_id&0xFFFF;	buf += sizeof(short);				//prerequisition race
	*(short*)buf = ess->require_strength;		buf += sizeof(short);				//prerequisition strength
	*(short*)buf = ess->require_tili;		buf += sizeof(short);				//prerequisition val
	*(short*)buf = ess->require_agility;		buf += sizeof(short);				//prerequisition agility
	*(short*)buf = ess->require_energy;		buf += sizeof(short);				//prerequisition energy

	int temp2 = element_data::RandNormal(ess->durability_min, ess->durability_max, cls,element_data::LOWER_TREND);
	int temp;
	if(normal_addon != element_data::ADDON_LIST_DROP || ess->proc_type & 0x1000)
	{
		temp = temp2;
	}
	else
	{
		temp = element_data::RandNormal(ess->durability_drop_min, ess->durability_drop_max, cls,element_data::UPPER_TREND);
		if(temp > temp2) temp = temp2;
	}
	*(int*)buf = temp;						buf += sizeof(int);				//prerequisition durability
	*(int*)buf = temp2;						buf += sizeof(int);				//prerequisition max_durability

	//���������߱�ǩע���ǩ���ڱ����С�ͱ�������֮��
	*(short*)buf = sizeof(_armor_essence);	buf += sizeof(short);				//װ�������С���ֽڣ�?
	memcpy(buf,tag,tag_size);
	buf += tag_size;

	// armor_essence ����
	char * essence_ptr = buf;
	*(int*)buf = element_data::RandNormal(ess->defence_low, ess->defence_high, cls,element_data::LOWER_TREND);				buf += sizeof(int);
	*(int*)buf = element_data::RandNormal(ess->armor_enhance_low, ess->armor_enhance_high, cls,element_data::LOWER_TREND);	buf += sizeof(int);
	*(int*)buf = element_data::RandNormal(ess->mp_enhance_low, ess->mp_enhance_high, cls,element_data::LOWER_TREND);			buf += sizeof(int);
	*(int*)buf = element_data::RandNormal(ess->hp_enhance_low, ess->hp_enhance_high, cls,element_data::LOWER_TREND);			buf += sizeof(int);

	//����
	int res[5] = {0,0,0,0,0};
	generate_magic_defense(res,(int*)(ess->magic_defences), cls, ess->force_all_magic_defences || ess->fixed_props!=0);
	
	*(int*)buf = res[0];		buf += sizeof(int);
	*(int*)buf = res[1];		buf += sizeof(int);
	*(int*)buf = res[2];		buf += sizeof(int);
	*(int*)buf = res[3];		buf += sizeof(int);
	*(int*)buf = res[4];		buf += sizeof(int);

	// �׶�
	*(short*)buf = hole_num;				buf += sizeof(short);				//�׶�����Ŀ
	*(short*)buf = 0;					buf += sizeof(short);				//�׶���������������
	for(i=0; i<hole_num; i++)
	{
		*(int*)buf = 0;					buf += sizeof(int);				//�׶���Ƕ��������ͣ�û�ж���?0
	}

	// addon
	*(int*)buf = addon_num;		buf += sizeof(int);
	char * addon_sld = addon_buf;
	for(i=0; i<addon_num; i++)
	{		
		int id = *(int*)addon_sld;
		addon_update_ess_data(*(addon_data*)addon_sld, essence_ptr, sizeof(_armor_essence), (prerequisition*)require_ptr);
		addon_sld += ((id & 0x6000)>>13)*sizeof(int)+sizeof(int);
	}	
	update_require_data((prerequisition*)require_ptr);
	memcpy(buf, addon_buf, addon_size);
	set_to_classid(DT_ARMOR_ESSENCE, (item_data*)(*data), -1);

	return 0;
}

template <typename RAND_CLASS>
int generate_new_armor(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls,element_data::GEN_ADDON_MODE normal_addon,const void * tag,unsigned int tag_size,int * sa_list = NULL)
{
	DATA_TYPE datatype;
	unsigned int i=0;
	NEW_ARMOR_ESSENCE * ess = (NEW_ARMOR_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_NEW_ARMOR_ESSENCE)	return -1;

	size = sizeof(item_data) + sizeof(_item_content) + sizeof(_new_armor_essence) + sizeof(short);
	unsigned int hole_num = 0;
	if(normal_addon == element_data::ADDON_LIST_DROP)
	{
		hole_num = element_data::RandSelect(&(ess->drop_probability_socket0), sizeof(float), 5, cls,element_data::LOWER_TREND);		//�׶�����Ŀ
	}
	else 
	{
		hole_num = element_data::RandSelect(&(ess->make_probability_socket0), sizeof(float), 5, cls,element_data::LOWER_TREND);		//�׶�����Ŀ
	}
	size += hole_num*sizeof(int);

	ASSERT(tag_size >= sizeof(short));
	size += tag_size;

	
	// ���addons
	char addon_buf[ELEMENTDATAMAN_MAX_NUM_ADDONS*sizeof(_addon)];
	unsigned int addon_num = element_data::RandSelect(&(ess->probability_addon_num0),sizeof(float), 5, cls,element_data::LOWER_TREND);	//���Ա���Ŀ����Ŀ

	unsigned int addon_size = 0;
	if(ess->fixed_props)
	{
		addon_size = generate_equipment_addon_buffer_2(DT_NEW_ARMOR_ESSENCE, (int*)&(ess->addons[0]), sizeof(int)+sizeof(float), 32, addon_buf,addon_num);
	}
	else
	{
		if(normal_addon == element_data::ADDON_LIST_DROP)
			addon_size = generate_equipment_addon_buffer(DT_NEW_ARMOR_ESSENCE, (char*)&(ess->addons[0]), 32, addon_buf,addon_num);
		else if(normal_addon == element_data::ADDON_LIST_PRODUCE)
			addon_size = generate_equipment_addon_buffer(DT_NEW_ARMOR_ESSENCE, (char*)&(ess->rands[0]), 32, addon_buf,addon_num);
		else  if (normal_addon == element_data::ADDON_LIST_SPEC)
		{
			addon_size = generate_spec_addon_buffer(DT_NEW_ARMOR_ESSENCE,addon_buf,ELEMENTDATAMAN_MAX_NUM_ADDONS,addon_num,sa_list);
		}
		else
		{
			addon_size = 0;
			addon_num = 0;
		}
	}
	size += addon_size;
		
	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);


	*(unsigned int*)buf = id;				buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;						buf += sizeof(unsigned int);			//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);			//��Ʒ�Ķѵ�����
	NEW_ARMOR_SUB_TYPE* sub_type = (NEW_ARMOR_SUB_TYPE*)get_data_ptr(ess->id_sub_type, ID_SPACE_ESSENCE, datatype);
	if(sub_type == NULL || datatype != DT_NEW_ARMOR_SUB_TYPE)				// error
		return -1;
	*(int*)buf = sub_type->equip_mask|(addon_num?ELEMENTDATAMAN_EQUIP_MASK_HAS_ADDON:0);		buf += sizeof(int);				//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);				//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_NEW_ARMOR_ESSENCE;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;				buf += sizeof(int);				//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	// prerequisition
	char * require_ptr = buf;
	*(short*)buf = ess->require_level;		buf += sizeof(short);				//prerequisition level
	*(short*)buf = ess->character_combo_id&0xFFFF;	buf += sizeof(short);				//prerequisition race
	*(short*)buf = ess->require_strength;		buf += sizeof(short);				//prerequisition strength
	*(short*)buf = ess->require_tili;		buf += sizeof(short);				//prerequisition val
	*(short*)buf = ess->require_agility;		buf += sizeof(short);				//prerequisition agility
	*(short*)buf = ess->require_energy;		buf += sizeof(short);				//prerequisition energy

	int temp2 = element_data::RandNormal(ess->durability_min, ess->durability_max, cls,element_data::LOWER_TREND);
	int temp;
	if(normal_addon != element_data::ADDON_LIST_DROP || ess->proc_type & 0x1000)
	{
		temp = temp2;
	}
	else
	{
		temp = element_data::RandNormal(ess->durability_drop_min, ess->durability_drop_max, cls,element_data::UPPER_TREND);
		if(temp > temp2) temp = temp2;
	}
	*(int*)buf = temp;						buf += sizeof(int);				//prerequisition durability
	*(int*)buf = temp2;						buf += sizeof(int);				//prerequisition max_durability

	*(short*)buf = sizeof(_new_armor_essence);	buf += sizeof(short);				//װ�������С���ֽڣ�?
	memcpy(buf,tag,tag_size);
	buf += tag_size;

	char * essence_ptr = buf;
	*(int*)buf = element_data::RandNormal(ess->defence_low, ess->defence_high, cls,element_data::LOWER_TREND);				buf += sizeof(int);
	*(int*)buf = element_data::RandNormal(ess->armor_enhance_low, ess->armor_enhance_high, cls,element_data::LOWER_TREND);	buf += sizeof(int);
	*(int*)buf = element_data::RandNormal(ess->mp_enhance_low, ess->mp_enhance_high, cls,element_data::LOWER_TREND);			buf += sizeof(int);
	*(int*)buf = element_data::RandNormal(ess->hp_enhance_low, ess->hp_enhance_high, cls,element_data::LOWER_TREND);			buf += sizeof(int);

	int res[5] = {0,0,0,0,0};
	generate_magic_defense(res,(int*)(ess->magic_defences), cls, ess->force_all_magic_defences || ess->fixed_props!=0);
	
	*(int*)buf = res[0];		buf += sizeof(int);
	*(int*)buf = res[1];		buf += sizeof(int);
	*(int*)buf = res[2];		buf += sizeof(int);
	*(int*)buf = res[3];		buf += sizeof(int);
	*(int*)buf = res[4];		buf += sizeof(int);

	*(short*)buf = hole_num;				buf += sizeof(short);
	*(short*)buf = 0;					buf += sizeof(short);
	for(i=0; i<hole_num; i++)
	{
		*(int*)buf = 0;					buf += sizeof(int);
	}

	// addon
	*(int*)buf = 0;		buf += sizeof(int);
	// new struct new_armor_essence
	*(char*)buf = 0;		buf += sizeof(char);
	*(char*)buf = 0;		buf += sizeof(char);
	
	/*char * addon_sld = addon_buf;
	for(i=0; i<addon_num; i++)
	{		
		int id = *(int*)addon_sld;
		addon_update_ess_data(*(addon_data*)addon_sld, essence_ptr, sizeof(_new_armor_essence), (prerequisition*)require_ptr);
		addon_sld += ((id & 0x6000)>>13)*sizeof(int)+sizeof(int);
	}*/

	update_require_data((prerequisition*)require_ptr);
	//memcpy(buf, addon_buf, addon_size);
	set_to_classid(DT_NEW_ARMOR_ESSENCE, (item_data*)(*data), -1);

	return 0;
}
template <typename RAND_CLASS>
int generate_projectile(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	PROJECTILE_ESSENCE * ess = (PROJECTILE_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_PROJECTILE_ESSENCE)	return -1;

	size = sizeof(item_data) + sizeof(_item_content) + sizeof(_projectile_essence);
	// �޿׶�
	// �̶�addons 4��
	char addon_buf[ELEMENTDATAMAN_MAX_NUM_ADDONS*sizeof(_addon)];
	unsigned int addon_num = 4;		//���Ա���Ŀ����Ŀ�̶� 4

	unsigned int addon_size = generate_equipment_addon_buffer_2(DT_PROJECTILE_ESSENCE, (int*)&(ess->id_addon0),sizeof(int), 4, addon_buf,addon_num);
	size += addon_size;

	//����tag size
	size += sizeof(short);

	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = *data;

	*(unsigned int*)buf = id;				buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;						buf += sizeof(unsigned int);			//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);			//��Ʒ�Ķѵ�����
	*(int*)buf = ELEMENTDATAMAN_EQUIP_MASK_PROJECTILE|(addon_num?0x40000000:0);		buf += sizeof(int);				//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);				//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_PROJECTILE_ESSENCE;		buf += sizeof(int);				//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;				buf += sizeof(int);				//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	// prerequisition
	char * require_ptr = buf;
	//����int����6��
	*(short*)buf = 0;		buf += sizeof(short);				//prerequisition level
	*(short*)buf = (short)0xFFFF;	buf += sizeof(short);					
	*(short*)buf = 0;		buf += sizeof(short);				//prerequisition strength
	*(short*)buf = 0;		buf += sizeof(short);				
	*(short*)buf = 0;		buf += sizeof(short);				//prerequisition agility
	*(short*)buf = 0;		buf += sizeof(short);				

	*(int*)buf = 1;			buf += sizeof(int);				//prerequisition durability
	*(int*)buf = 1;			buf += sizeof(int);				//prerequisition max_durability
	*(short*)buf = sizeof(_projectile_essence);	buf += sizeof(short);				//װ�������С���ֽڣ�?
	*(char*)buf = element_data::IMT_NULL;buf += sizeof(char);
	*(char*)buf = 0;		buf += sizeof(char);
	
	// projectile_essence ����
	char * essence_ptr = buf;
	*(int*)buf = ess->type;					buf += sizeof(int);			//��ҩ����
	*(int*)buf = ess->damage_enhance;		buf += sizeof(int);			//���������Ĺ�����
	*(int*)buf = ess->damage_scale_enhance; 	buf += sizeof(int);			//���ձ������ӹ�����
	*(int*)buf = ess->require_weapon_level_min;	buf += sizeof(int);			//��Ҫ�����ȼ�
	*(int*)buf = ess->require_weapon_level_max; 	buf += sizeof(int);			//��Ҫ�����ȼ�

	// �׶� 
	*(short*)buf = 0;		buf += sizeof(short);
	*(short*)buf = 0;		buf += sizeof(short);


	// addon
	*(int*)buf = addon_num;		buf += sizeof(int);

	char * addon_sld = addon_buf;
	for(unsigned int i=0; i<addon_num; i++)
	{
		int id = *(int*)addon_sld;
		addon_update_ess_data(*(addon_data*)addon_sld, essence_ptr, sizeof(_projectile_essence), (prerequisition*)require_ptr);
		addon_sld += ((id & 0x6000)>>13)*sizeof(int)+sizeof(int);
	}	
	update_require_data((prerequisition*)require_ptr);
	memcpy(buf, addon_buf, addon_size);
	set_to_classid(DT_PROJECTILE_ESSENCE, (item_data*)(*data), -1);

	return 0;
}
template <typename RAND_CLASS>
int generate_quiver(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	QUIVER_ESSENCE * ess = (QUIVER_ESSENCE *)get_data_ptr(id, ID_SPACE_ESSENCE, datatype);
	if(ess == NULL || datatype != DT_QUIVER_ESSENCE)	return -1;
	datatype = DT_PROJECTILE_ESSENCE;
	//���ﲻ����dup��ʽ
//	if(generate_projectile(ess->id_projectile, ID_SPACE_ESSENCE, (char **)data, size, cls) == 0)
	int ret = duplicate_static_item(id, data,size);
	if(ret == 0)
	{
		ASSERT(ess->num_max < (int)((item_data*)(*data))->pile_limit);
		((item_data*)(*data))->count = element_data::RandNormal(ess->num_min,ess->num_max,cls,element_data::LOWER_TREND);
		return 0;
	}
	else
		return -1;
/*	{
		ASSERT(ess->num_max < (int)((item_data*)(*data))->pile_limit);
		((item_data*)(*data))->count = element_data::RandNormal<RAND_CLASS,element_data::LOWER>(ess->num_min,ess->num_max,cls);
		return 0;
	}
	else
		return -1;
		*/
}


template <typename RAND_CLASS>
int generate_quiver_for_sell(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	QUIVER_ESSENCE * ess = (QUIVER_ESSENCE *)get_data_ptr(id, ID_SPACE_ESSENCE, datatype);
	if(ess == NULL || datatype != DT_QUIVER_ESSENCE)	return -1;
	datatype = DT_PROJECTILE_ESSENCE;
	//�����޷���dup����Ϊ����
	if(generate_projectile(ess->id_projectile, ID_SPACE_ESSENCE, (char **)data, size, cls) == 0)
	{
		ASSERT(ess->num_max < (int)((item_data*)(*data))->pile_limit);
		((item_data*)(*data))->count = element_data::RandNormal(ess->num_min,ess->num_max,cls,element_data::LOWER_TREND);
		return 0;
	}
	else
		return -1;
}

template <typename RAND_CLASS>
int generate_decoration(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls,element_data::GEN_ADDON_MODE normal_addon,const void * tag,unsigned int tag_size, int * sa_list = NULL)
{
	DATA_TYPE datatype;
	unsigned int i=0;
	DECORATION_ESSENCE * ess = (DECORATION_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_DECORATION_ESSENCE)	return -1;
	
	size = sizeof(item_data) + sizeof(_item_content) + sizeof(_decoration_essence);
	// �׶�
	unsigned int hole_num = 0;		//�׶�����Ŀ
	
	// ���addons
	char addon_buf[ELEMENTDATAMAN_MAX_NUM_ADDONS*sizeof(_addon)];
	unsigned int addon_num = element_data::RandSelect(&(ess->probability_addon_num0),sizeof(float), 5,cls,element_data::LOWER_TREND);	//���Ա���Ŀ����Ŀ

	unsigned int addon_size = 0;
	if(ess->fixed_props)
	{
		addon_size = generate_equipment_addon_buffer_2(DT_DECORATION_ESSENCE, (int*)&(ess->addons[0]), sizeof(int)+sizeof(float),32, addon_buf,addon_num);
	}
	else
	{
		if(normal_addon == element_data::ADDON_LIST_DROP)
			addon_size = generate_equipment_addon_buffer(DT_DECORATION_ESSENCE, (char*)&(ess->addons[0]), 32, addon_buf,addon_num);
		else if(normal_addon == element_data::ADDON_LIST_PRODUCE)
			addon_size = generate_equipment_addon_buffer(DT_DECORATION_ESSENCE, (char*)&(ess->rands[0]), 32, addon_buf,addon_num);
		else  if (normal_addon == element_data::ADDON_LIST_SPEC)
		{
			addon_size = generate_spec_addon_buffer(DT_DECORATION_ESSENCE,addon_buf,ELEMENTDATAMAN_MAX_NUM_ADDONS,addon_num,sa_list);
		}
		else
		{
			addon_size = 0;
			addon_num = 0;
		}
	}
	size += addon_size;

	//����tag size
	ASSERT(tag_size >= sizeof(short));
	size += tag_size;

	
	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);
	
	
	*(unsigned int*)buf = id;				buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;						buf += sizeof(unsigned int);			//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);			//��Ʒ�Ķѵ�����
	DECORATION_SUB_TYPE* sub_type = (DECORATION_SUB_TYPE*)get_data_ptr(ess->id_sub_type, ID_SPACE_ESSENCE, datatype);
	if(sub_type == NULL || datatype != DT_DECORATION_SUB_TYPE)				// error
		return -1;
	*(int*)buf = sub_type->equip_mask|(addon_num?ELEMENTDATAMAN_EQUIP_MASK_HAS_ADDON:0);		buf += sizeof(int);				//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);				//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_DECORATION_ESSENCE;		buf += sizeof(int);				//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;				buf += sizeof(int);				//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;
	
	// prerequisition
	char * require_ptr = buf;
	*(short*)buf = ess->require_level;		buf += sizeof(short);				//prerequisition level
	*(short*)buf = ess->character_combo_id&0xFFFF;	buf += sizeof(short);				//prerequisition race
	*(short*)buf = ess->require_strength;		buf += sizeof(short);				//prerequisition strength
	*(short*)buf = ess->require_tili;		buf += sizeof(short);				//prerequisition val
	*(short*)buf = ess->require_agility;		buf += sizeof(short);				//prerequisition agility
	*(short*)buf = ess->require_energy;		buf += sizeof(short);				//prerequisition energy

	int temp2 = element_data::RandNormal(ess->durability_min, ess->durability_max, cls,element_data::LOWER_TREND);
	int temp;
	if(normal_addon != element_data::ADDON_LIST_DROP || ess->proc_type & 0x1000)
	{
		temp = temp2;
	}
	else
	{
		temp = element_data::RandNormal(ess->durability_drop_min, ess->durability_drop_max, cls,element_data::UPPER_TREND);
		if(temp > temp2) temp = temp2;
	}
	*(int*)buf = temp;						buf += sizeof(int);				//prerequisition durability
	*(int*)buf = temp2;						buf += sizeof(int);				//prerequisition max_durability

	//���������߱�ǩע���ǩ���ڱ����С�ͱ�������֮��
	*(short*)buf = sizeof(_decoration_essence);	buf += sizeof(short);				//װ�������С���ֽڣ�?
	memcpy(buf,tag,tag_size);
	buf += tag_size;

	
	// ����
	char * essence_ptr = buf;
	*(int*)buf = element_data::RandNormal(ess->damage_low, ess->damage_high, cls,element_data::LOWER_TREND);				buf += sizeof(int);	//int damage;
	*(int*)buf = element_data::RandNormal(ess->magic_damage_low, ess->magic_damage_high, cls,element_data::LOWER_TREND);	buf += sizeof(int);	//int magic_damage;
	*(int*)buf = element_data::RandNormal(ess->defence_low, ess->defence_high, cls,element_data::LOWER_TREND);			buf += sizeof(int);	//int defense;
	*(int*)buf = element_data::RandNormal(ess->armor_enhance_low, ess->armor_enhance_high, cls,element_data::LOWER_TREND); buf += sizeof(int); //int armor;

	//����
	int res[5] = {0,0,0,0,0};
	generate_magic_defense(res,(int*)(ess->magic_defences),cls,ess->fixed_props!=0);
	
	*(int*)buf = res[0];		buf += sizeof(int);
	*(int*)buf = res[1];		buf += sizeof(int);
	*(int*)buf = res[2];		buf += sizeof(int);
	*(int*)buf = res[3];		buf += sizeof(int);
	*(int*)buf = res[4];		buf += sizeof(int);

	// �׶�
	*(short*)buf = hole_num;		buf += sizeof(short);				//�׶�����Ŀ
	*(short*)buf = 0;			buf += sizeof(short);				//�׶���������������
	
	// addon
	*(int*)buf = addon_num;		buf += sizeof(int);
	char * addon_sld = addon_buf;
	for(i=0; i<addon_num; i++)
	{
		int id = *(int*)addon_sld;
		addon_update_ess_data(*(addon_data*)addon_sld, essence_ptr, sizeof(_decoration_essence), (prerequisition*)require_ptr);
		addon_sld += ((id & 0x6000)>>13)*sizeof(int)+sizeof(int);
	}
	update_require_data((prerequisition*)require_ptr);
	memcpy(buf, addon_buf, addon_size);
	set_to_classid(DT_DECORATION_ESSENCE, (item_data*)(*data), -1);
	
	return 0;
}

template <typename RAND_CLASS>
int generate_stone(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	STONE_ESSENCE * ess = (STONE_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_STONE_ESSENCE)	return -1;

	char addon_buf[ELEMENTDATAMAN_MAX_NUM_ADDONS*sizeof(_addon)];
	int *pBuf = (int*)addon_buf;
    unsigned int addon_size = 0;

    if (ess->id_addon_damage != 0)
    {
	    *pBuf = 1;	//only one weapon addon now
	    addon_size = generate_addon_buffer(datatype,ess->id_addon_damage, (char*)(pBuf + 1));
	    if(addon_size)
	    {
		    pBuf = (int*)(((char*)(pBuf+1)) + addon_size);
	    }
	    else
	    {
		    //ASSERT(false);
		    return -1;
	    }
    }
    else
    {
        *pBuf++ = 0;
    }

    if (ess->id_addon_defence != 0)
    {
        *pBuf = 1;	//only one armor addon now
	    addon_size = generate_addon_buffer(datatype,ess->id_addon_defence, (char*)(pBuf + 1));
	    if(addon_size)
	    {
		    pBuf = (int*)(((char*)(pBuf+1)) + addon_size);
	    }
	    else
	    {
		    //ASSERT(false);
		    return -1;
	    }
    }
    else
    {
        *pBuf++ = 0;
    }

	unsigned int ess_size = ((char*)pBuf) - addon_buf;
	size = sizeof(item_data) + ess_size;

	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char *buf = *data;
	*(unsigned int*)buf = id;	buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;		buf += sizeof(unsigned int);		//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;buf += sizeof(unsigned int);	//��Ʒ�Ķѵ�����
	*(int*)buf = 0;		buf += sizeof(int);		//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;	buf += sizeof(int);				//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_STONE_ESSENCE;	buf += sizeof(int);				//��Ʒ��Ӧ�����ID	
	*(int*)buf = 0;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	*(int*)buf = 0;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	*(int*)buf = ess->price;	buf += sizeof(int);				//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = size - (buf - (char*)(*data));
	*item_content = buf;
	memcpy(buf, addon_buf, ess_size);
	set_to_classid(DT_STONE_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_medicine(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	MEDICINE_ESSENCE * ess = (MEDICINE_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_MEDICINE_ESSENCE)	return -1;

	size = sizeof(item_data);

	switch(ess->id_major_type)
	{
	case 1794:	// ��Ѫ����
	case 1802:	// ��ħ����
	case 1810:	// ��Ѫ��ħ����
		size += sizeof(unsigned int) + sizeof(int) + sizeof(int) + sizeof(int);
		break;
	case 1815:	// �ⶾҩ
	case 2038:	// ˲��ⶾ�?
		size += sizeof(int) + sizeof(int);
		break;
	default:
		return -1;
	}
		
	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;				buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;						buf += sizeof(unsigned int);			//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);			//��Ʒ�Ķѵ�����
	*(int*)buf = 0;							buf += sizeof(int);				//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);				//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_MEDICINE_ESSENCE;		buf += sizeof(int);				//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;				buf += sizeof(int);				//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	switch(ess->id_major_type)
	{
	case 1794:	// ��Ѫ����
		if(ess->cool_time < 1000) ess->cool_time *= 100;
		*(int*)buf = ess->hp_add_total;		buf += sizeof(int);
		*(int*)buf = ess->hp_add_time;		buf += sizeof(int);
		*(int*)buf = ess->cool_time;		buf += sizeof(int);
		*(int*)buf = ess->require_level;	buf += sizeof(int);
		ASSERT(ess->hp_add_time && ess->hp_add_total);
		ASSERT(ess->hp_add_total/ess->hp_add_time);
		break;
	case 1802:	// ��ħ����
		if(ess->cool_time < 1000) ess->cool_time *= 100;
		*(int*)buf = ess->mp_add_total;		buf += sizeof(int);
		*(int*)buf = ess->mp_add_time;		buf += sizeof(int);
		*(int*)buf = ess->cool_time;		buf += sizeof(int);
		*(int*)buf = ess->require_level;	buf += sizeof(int);
		ASSERT(ess->mp_add_time && ess->mp_add_total);
		ASSERT(ess->mp_add_total/ess->mp_add_time);
		break;
	case 1810:	// ��Ѫ��ħ����
		if(ess->cool_time < 1000) ess->cool_time *= 100;
		*(int*)buf = ess->mp_add_total;		buf += sizeof(int);
		*(int*)buf = ess->hp_add_total;		buf += sizeof(int);
		*(int*)buf = ess->cool_time;		buf += sizeof(int);
		*(int*)buf = ess->require_level;	buf += sizeof(int);
		ASSERT(ess->hp_add_time || ess->mp_add_total);
		break;
	case 1815:	// �ⶾҩ
	case 2038:	// ˲��ⶾ�?
		if(ess->cool_time < 1000) ess->cool_time *= 100;
		*(int*)buf = ess->cool_time;		buf += sizeof(int);
		*(int*)buf = ess->require_level;	buf += sizeof(int);
		break;
	default:
		return -1;
	}
	set_to_classid(DT_MEDICINE_ESSENCE, (item_data*)(*data), ess->id_major_type);

	return 0;
}

template <typename ESSENCE >
int generate_material(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, ESSENCE * p, DATA_TYPE DTYPE)
{
	DATA_TYPE datatype;
	ESSENCE * ess = (ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DTYPE)	return -1;

	size = sizeof(item_data) ;
		
	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;				buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;						buf += sizeof(unsigned int);			//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);			//��Ʒ�Ķѵ�����
	*(int*)buf = 0;							buf += sizeof(int);				//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);				//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DTYPE;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;				buf += sizeof(int);				//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	set_to_classid(DTYPE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_skilltome(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	SKILLTOME_ESSENCE * ess = (SKILLTOME_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_SKILLTOME_ESSENCE)	return -1;

	size = sizeof(item_data);
		
	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;				buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;						buf += sizeof(unsigned int);			//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);			//��Ʒ�Ķѵ�����
	*(int*)buf = 0;							buf += sizeof(int);				//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);				//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_SKILLTOME_ESSENCE;		buf += sizeof(int);				//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;				buf += sizeof(int);				//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	set_to_classid(DT_SKILLTOME_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_element(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	ELEMENT_ESSENCE * ess = (ELEMENT_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_ELEMENT_ESSENCE)	return -1;

	size = sizeof(item_data);

	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;				buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;						buf += sizeof(unsigned int);			//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);			//��Ʒ�Ķѵ�����
	*(int*)buf = 0;							buf += sizeof(int);				//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);				//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_ELEMENT_ESSENCE;		buf += sizeof(int);				//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;				buf += sizeof(int);				//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	set_to_classid(DT_ELEMENT_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_flysword(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls,const void * tag,unsigned int tag_size)
{

	DATA_TYPE datatype;
	FLYSWORD_ESSENCE * ess = (FLYSWORD_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_FLYSWORD_ESSENCE)	return -1;

	size = sizeof(item_data) + sizeof(_cls_flysword_essence);

	//���������߱�ǩ
	ASSERT(tag_size >= 2);
	size += tag_size;

	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;			buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;				buf += sizeof(unsigned int);		//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);		//��Ʒ�Ķѵ�����
	*(int*)buf = ELEMENTDATAMAN_EQUIP_MASK_FLYSWORD;buf += sizeof(int);		//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);		//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_FLYSWORD_ESSENCE;		buf += sizeof(int);		//��Ʒ��Ӧ�����ID
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);		//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;				buf += sizeof(int);		//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;			buf += sizeof(int);		//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;		buf += sizeof(unsigned int);		//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;		buf += sizeof(char *);		//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	int t =	element_data::RandNormal((int)ess->time_max_min, (int)ess->time_max_max, cls,element_data::LOWER_TREND);
	
	// essence
	*(int*)buf = (int)(t*0.5f);			buf += sizeof(int);
	*(int*)buf = (int)t;				buf += sizeof(unsigned int);
	*(short*)buf = ess->require_player_level_min;	buf += sizeof(short);
	*(char*)buf = ess->level;			buf += sizeof(char);
	*(char*)buf = 0;					buf += sizeof(char);
	*(int*)buf = ess->character_combo_id;		buf += sizeof(int);	
	*(unsigned int*)buf = (unsigned int)ess->time_increase_per_element;	buf += sizeof(int);
	
	float speed = element_data::Rand(ess->speed_increase_min, ess->speed_increase_max, cls,element_data::LOWER_TREND);
	*(float*)buf = speed;				buf += sizeof(float);
	speed = element_data::Rand(ess->speed_rush_increase_min, ess->speed_rush_increase_max, cls,element_data::LOWER_TREND);
	*(float*)buf = speed;				buf += sizeof(float);

	//���������߱�ǩ
	memcpy(buf,tag,tag_size);
	buf += tag_size;

	set_to_classid(DT_FLYSWORD_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_wingmanwing(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls,const void * tag,unsigned int tag_size)
{
	DATA_TYPE datatype;
	WINGMANWING_ESSENCE * ess = (WINGMANWING_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_WINGMANWING_ESSENCE)	return -1;

	size = sizeof(item_data) + sizeof(_wingmanwing_essence);

	ASSERT(tag_size >= 2);
	size += tag_size;

	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;			buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;				buf += sizeof(unsigned int);		//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);		//��Ʒ�Ķѵ�����
	*(int*)buf = ELEMENTDATAMAN_EQUIP_MASK_FLYSWORD;buf += sizeof(int);		//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);		//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_WINGMANWING_ESSENCE;		buf += sizeof(int);		//��Ʒ��Ӧ�����ID
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);		//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;				buf += sizeof(int);		//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;			buf += sizeof(int);		//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;		buf += sizeof(unsigned int);		//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;		buf += sizeof(char *);		//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	// essence
	*(int*)buf = ess->require_player_level_min;	buf += sizeof(int);
	*(unsigned int*)buf = ess->mp_launch;			buf += sizeof(unsigned int);
	*(unsigned int*)buf = ess->mp_per_second;		buf += sizeof(unsigned int);
	float inc_speed = ess->speed_increase; 
	if(inc_speed >10.f) inc_speed = 10.f;
	*(float*)buf = inc_speed;			buf += sizeof(int);

	//���������߱�ǩ
	memcpy(buf,tag,tag_size);
	buf += tag_size;

	set_to_classid(DT_WINGMANWING_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_townscroll(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	TOWNSCROLL_ESSENCE * ess = (TOWNSCROLL_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_TOWNSCROLL_ESSENCE)	return -1;

	size = sizeof(item_data) + sizeof(_townscroll_essence);
		
	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;			buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;				buf += sizeof(unsigned int);		//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);		//��Ʒ�Ķѵ�����
	*(int*)buf = 0;					buf += sizeof(int);		//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);		//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_TOWNSCROLL_ESSENCE;		buf += sizeof(int);		//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);		//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;				buf += sizeof(int);		//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;			buf += sizeof(int);		//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	ASSERT(ess->use_time > 1e-3 && ess->use_time < 200);
	*(unsigned int*)buf = (int )(ess->use_time * 20);	buf += sizeof(int);		//ʹ��ʱ��
	
	set_to_classid(DT_TOWNSCROLL_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_damagerune(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	DAMAGERUNE_ESSENCE * ess = (DAMAGERUNE_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_DAMAGERUNE_ESSENCE)	return -1;

	size = sizeof(item_data) + sizeof(_damagerune_essence);
		
	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;				buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;				buf += sizeof(unsigned int);			//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);			//��Ʒ�Ķѵ�����
	*(int*)buf = ELEMENTDATAMAN_EQUIP_MASK_DAMAGERUNE;buf += sizeof(int);				//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);				//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_DAMAGERUNE_ESSENCE;		buf += sizeof(int);				//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;				buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;				buf += sizeof(int);				//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	// ����
	*(short*)buf = ess->damage_type;		buf += sizeof(short);
	*(short*)buf = ess->require_weapon_level_min;	buf += sizeof(short);
	*(short*)buf = ess->require_weapon_level_max;	buf += sizeof(short);
	*(short*)buf = ess->damage_increased;		buf += sizeof(short);

	set_to_classid(DT_DAMAGERUNE_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_armorrune(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	ARMORRUNE_ESSENCE * ess = (ARMORRUNE_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_ARMORRUNE_ESSENCE)	return -1;

	size = sizeof(item_data) + sizeof(_armorrune_essence);
		
	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;				buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;						buf += sizeof(unsigned int);			//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);			//��Ʒ�Ķѵ�����
	*(int*)buf = 0;							buf += sizeof(int);				//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);				//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_ARMORRUNE_ESSENCE;		buf += sizeof(int);				//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;				buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;			buf += sizeof(int);				//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;		buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;		buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	// ����
	*(int*)buf = ess->damage_type;			buf += sizeof(int);		//6��ʹ��ʱ��
	*(int*)buf = ess->require_player_level_max;	buf += sizeof(int);
	*(float*)buf = ess->damage_reduce_percent;	buf += sizeof(float);
	*(int*)buf = ess->damage_reduce_time;		buf += sizeof(int);

	set_to_classid(DT_ARMORRUNE_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_tossmatter(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	TOSSMATTER_ESSENCE * ess = (TOSSMATTER_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_TOSSMATTER_ESSENCE)	return -1;

	size = sizeof(item_data) + sizeof(_tossmatter_essence);

	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;				buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;						buf += sizeof(unsigned int);			//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);			//��Ʒ�Ķѵ�����
	*(int*)buf = 0;							buf += sizeof(int);				//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);				//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_TOSSMATTER_ESSENCE;		buf += sizeof(int);				//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = 0;							buf += sizeof(int);				//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	// prerequisition
	*(int*)buf = ess->require_level;		buf += sizeof(int);				//prerequisition level
	*(int*)buf = ess->require_strength;		buf += sizeof(int);				//prerequisition strength
	*(int*)buf = ess->require_agility;		buf += sizeof(int);				//prerequisition agility
	*(int*)buf = ess->damage_low;			buf += sizeof(int);				//prerequisition durability
	*(int*)buf = ess->damage_high_min;		buf += sizeof(int);				//prerequisition max_durability
	*(float*)buf = ess->attack_range;		buf += sizeof(float);
	ASSERT(ess->use_time > 1e-3 && ess->use_time < 200);
	*(unsigned int*)buf = (int )(ess->use_time * 20);	buf += sizeof(int); //3��ʹ��ʱ��

	set_to_classid(DT_TOSSMATTER_ESSENCE, (item_data*)(*data), -1);
	return 0;
}


template <typename RAND_CLASS>
int generate_taskmatter(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	TASKMATTER_ESSENCE * ess = (TASKMATTER_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_TASKMATTER_ESSENCE)	return -1;

	size = sizeof(item_data) + sizeof(_taskmatter_essence);
	
	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;				buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;						buf += sizeof(unsigned int);			//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);			//��Ʒ�Ķѵ�����
	*(int*)buf = 0;							buf += sizeof(int);				//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);				//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_TASKMATTER_ESSENCE;		buf += sizeof(int);				//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = 0;							buf += sizeof(int);				//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	set_to_classid(DT_TASKMATTER_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_taskdice(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	TASKDICE_ESSENCE * ess = (TASKDICE_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_TASKDICE_ESSENCE)	return -1;

	size = sizeof(item_data) + sizeof(_taskdice_essence);
	unsigned int dice_index = element_data::RandSelect(&(ess->task_lists[0].probability), sizeof(int)+sizeof(float), sizeof(ess->task_lists)/sizeof(ess->task_lists[0]), cls,element_data::LOWER_TREND);		//�׶�����Ŀ
	
	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;			buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;				buf += sizeof(unsigned int);			//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);			//��Ʒ�Ķѵ�����
	*(int*)buf = 0;					buf += sizeof(int);				//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);				//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_TASKDICE_ESSENCE;		buf += sizeof(int);				//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;				buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = 0;					buf += sizeof(int);				//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	*(int*)buf = ess->task_lists[dice_index].id;	buf += sizeof(int);

	set_to_classid(DT_TASKDICE_ESSENCE, (item_data*)(*data), -1);
	return 0;
}
		  



//======================================================================
template <typename RAND_CLASS>
int generate_revivescroll(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	REVIVESCROLL_ESSENCE * ess = (REVIVESCROLL_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_REVIVESCROLL_ESSENCE)	return -1;

	size = sizeof(item_data) + sizeof(_revivescroll_essence);
		
	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;				buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;						buf += sizeof(unsigned int);			//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);			//��Ʒ�Ķѵ�����
	*(int*)buf = 0;							buf += sizeof(int);				//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);				//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_REVIVESCROLL_ESSENCE;	buf += sizeof(int);				//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;				buf += sizeof(int);				//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	
	set_to_classid(DT_REVIVESCROLL_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_unionscroll(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	UNIONSCROLL_ESSENCE * ess = (UNIONSCROLL_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_UNIONSCROLL_ESSENCE)	return -1;

	size = sizeof(item_data) + sizeof(_unionscroll_essence);
	

	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;				buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;						buf += sizeof(unsigned int);			//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);			//��Ʒ�Ķѵ�����
	*(int*)buf = 0;							buf += sizeof(int);				//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);				//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_UNIONSCROLL_ESSENCE;	buf += sizeof(int);				//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;				buf += sizeof(int);				//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	set_to_classid(DT_UNIONSCROLL_ESSENCE, (item_data*)(*data), -1);
	return 0;

}

template <typename RAND_CLASS>
int generate_tasknormalmatter(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	TASKNORMALMATTER_ESSENCE * ess = (TASKNORMALMATTER_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_TASKNORMALMATTER_ESSENCE)	return -1;

	size = sizeof(item_data);

	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;				buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;						buf += sizeof(unsigned int);			//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);			//��Ʒ�Ķѵ�����
	*(int*)buf = 0;							buf += sizeof(int);				//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);				//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_ELEMENT_ESSENCE;		buf += sizeof(int);				//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;				buf += sizeof(int);				//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	set_to_classid(DT_TASKNORMALMATTER_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_fashion_item(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls,const void * tag, unsigned int tag_size)
{	
	DATA_TYPE datatype;
	FASHION_ESSENCE * ess = (FASHION_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if( ess == NULL || datatype != DT_FASHION_ESSENCE)	return -1;

	size = sizeof(item_data) + sizeof(_fashion_essence);

	ASSERT(tag_size >= 2);
	size += tag_size;	

	FASHION_SUB_TYPE* sub_type = (FASHION_SUB_TYPE*)get_data_ptr(ess->id_sub_type, ID_SPACE_ESSENCE, datatype);
	if(sub_type == NULL || datatype != DT_FASHION_SUB_TYPE)				// error
		return -1;

	*data = (char *)abase::fastalloc(size);	
	char * buf = *data;
	
	*(unsigned int*)buf = id;			buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;				buf += sizeof(unsigned int);		//��Ʒ������	
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);		//��Ʒ�Ķѵ�����
	*(unsigned int*)buf = sub_type->equip_fashion_mask;	buf += sizeof(unsigned int);		//��Ʒ�Ŀ�װ����־���̶�ֵ����
	*(int*)buf = ess->proc_type;			buf += sizeof(int);		//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_FASHION_ESSENCE;		buf += sizeof(int);		//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);		//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;				buf += sizeof(int);		//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;			buf += sizeof(int);		//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char *)(*data)+size-buf;
	*item_content = buf;
	
	unsigned short color = 0;
    if(ess->combined_switch & FCS_RANDOM_COLOR_IN_RANGE)
    {
        float h,s,v;
		h = abase::Rand(ess->h_min, ess->h_max);
		s = abase::Rand(ess->s_min, ess->s_max);
		v = abase::Rand(ess->v_min, ess->v_max);
		int color_tmp = hsv2rgb(h,s,v);

		unsigned short r = ((color_tmp >> 16) >> 3) & 0x1F;
		unsigned short g = ((color_tmp >> 8) >> 3) & 0x1F;
		unsigned short b = (color_tmp >> 3) & 0x1F;
		color = ((r << 10) | (g << 5) | b) & 0x7FFF;
    }
    else
    {
        color = element_data::RandNormal(0, 0x7FFF, cls,element_data::ANY_TREND);
    }
	// prerequisition
	*(int*)buf = ess->require_level;	buf += sizeof(int);
	*(unsigned short*)buf = color;		buf += sizeof(short);
	*(unsigned short*)buf = (unsigned short)ess->gender;buf += sizeof(short);

	//���������߱�ǩ
	memcpy(buf,tag,tag_size);
	buf += tag_size;

	set_to_classid(DT_FASHION_ESSENCE, (item_data*)(*data), -1);	
	return 0;
}

template <typename RAND_CLASS>
int generate_fashion_new_item(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls,const void * tag, unsigned int tag_size)
{	
	DATA_TYPE datatype;
	FASHION_NEW_ESSENCE * ess = (FASHION_NEW_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if( ess == NULL || datatype != DT_FASHION_NEW_ESSENCE)	return -1;

	size = sizeof(item_data) + sizeof(_fashion_new_essence);

	ASSERT(tag_size >= 2);
	size += tag_size;	

	FASHION_NEW_SUB_TYPE* sub_type = (FASHION_NEW_SUB_TYPE*)get_data_ptr(ess->id_sub_type, ID_SPACE_ESSENCE, datatype);
	if(sub_type == NULL || datatype != DT_FASHION_NEW_SUB_TYPE)				// error
		return -1;

	*data = (char *)abase::fastalloc(size);	
	char * buf = *data;
	
	*(unsigned int*)buf = id;			buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;				buf += sizeof(unsigned int);		//��Ʒ������	
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);		//��Ʒ�Ķѵ�����
	*(int*)buf = sub_type->equip_fashion_mask;	buf += sizeof(int);		//��Ʒ�Ŀ�װ����־���̶�ֵ����
	*(int*)buf = ess->proc_type;			buf += sizeof(int);		//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_FASHION_NEW_ESSENCE;		buf += sizeof(int);		//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);		//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;				buf += sizeof(int);		//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;			buf += sizeof(int);		//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char *)(*data)+size-buf;
	*item_content = buf;
	
	// Anulando a cor
	unsigned short color = 0;
    
	// Pre requisitos
	*(unsigned short*)buf = ess->require_level;	buf += sizeof(short);
	*(unsigned short*)buf = ess->character_combo_id;		buf += sizeof(short);
	*(int*)buf = 0;	buf += sizeof(int);
	*(int*)buf = 0;	buf += sizeof(int);
	*(int*)buf = 100;	buf += sizeof(int); // Durability
	*(int*)buf = 100;	buf += sizeof(int); // Max_Durability
	*(unsigned short*)buf = 0;	buf += sizeof(short);
	*(unsigned short*)buf = 0;	buf += sizeof(short);
	*(int*)buf = 0;	buf += sizeof(int);
	*(unsigned short*)buf = color;		buf += sizeof(short);	
	*(unsigned short*)buf = (unsigned short)ess->gender;buf += sizeof(short);
	*(unsigned short*)buf = 0;	buf += sizeof(short);
	*(char*)buf = 0;	buf += sizeof(char);
	*(char*)buf = 0;	buf += sizeof(char);	

	memcpy(buf,tag,tag_size);
	buf += tag_size;

	set_to_classid(DT_FASHION_NEW_ESSENCE, (item_data*)(*data), -1);	
	return 0;
}


template <typename RAND_CLASS>
int generate_facepill(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{	
	DATA_TYPE datatype;
	FACEPILL_ESSENCE * ess = (FACEPILL_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_FACEPILL_ESSENCE)	return -1;

	size = sizeof(item_data) + sizeof(_facepill_essence);	
	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;			buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;				buf += sizeof(unsigned int);			//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);			//��Ʒ�Ķѵ�����
	*(int*)buf = 0;					buf += sizeof(int);				//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);				//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_FACEPILL_ESSENCE;		buf += sizeof(int);				//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;				buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;				buf += sizeof(int);				//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	*(int*)buf = ess->duration;	buf += sizeof(int);
	*(int*)buf = ess->character_combo_id; buf += sizeof(int);

	set_to_classid(DT_FACEPILL_ESSENCE, (item_data*)(*data), -1);
	return 0;
}


template <typename RAND_CLASS>
int generate_faceticket(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	FACETICKET_ESSENCE * ess = (FACETICKET_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_FACETICKET_ESSENCE)	return -1;

	size = sizeof(item_data);

	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;				buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;						buf += sizeof(unsigned int);			//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);			//��Ʒ�Ķѵ�����
	*(int*)buf = 0;							buf += sizeof(int);				//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);				//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_FACETICKET_ESSENCE;		buf += sizeof(int);				//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;				buf += sizeof(int);				//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;


	set_to_classid(DT_FACETICKET_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_gm_generator(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	GM_GENERATOR_ESSENCE * ess = (GM_GENERATOR_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_GM_GENERATOR_ESSENCE)	return -1;

	size = sizeof(item_data) + sizeof(int);

	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;				buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;						buf += sizeof(unsigned int);			//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);			//��Ʒ�Ķѵ�����
	*(int*)buf = 0;							buf += sizeof(int);				//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);				//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_GM_GENERATOR_ESSENCE;		buf += sizeof(int);				//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = 0;				buf += sizeof(int);				//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;
	*(int*) buf = ess->id_object;

	set_to_classid(DT_GM_GENERATOR_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_pet_egg(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	PET_EGG_ESSENCE * ess = (PET_EGG_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_PET_EGG_ESSENCE)	return -1;

	PET_ESSENCE * petess = (PET_ESSENCE *)get_data_ptr(ess->id_pet,idspace,datatype);
	if(petess == NULL || datatype != DT_PET_ESSENCE)	return -1;
	
	size = sizeof(item_data) + sizeof(_petegg_essence);

	//������ܵı䳤����?
	struct {
		int skill;
		int level;
	}skills[32];
	int skill_count = 0;
	int i;
	for(i = 0; i < 32; i ++)
	{
		if(ess->skills[i].id_skill <= 0 
			|| ess->skills[i].level <= 0 ) continue;
		skills[skill_count].skill = ess->skills[i].id_skill;
		skills[skill_count].level = ess->skills[i].level;
		skill_count ++;
	}

	int pet_class;
	switch(petess->id_type)
	{
		case 8781:      //  ���?
			pet_class = 0;
			break;
		case 8782:      //  ս������
			pet_class = 1;
			break;
		case 8783:      //  ���ͳ���
			pet_class = 2;
			break;
		case 28752:		// �ٻ���
			pet_class = 3;
			break;
		case 28913:		// ֲ��
			pet_class = 4;
			break;
		case 37698:		// ��������
			pet_class = 5;
			break;
		default:
			pet_class = 4;
			break;
	}
	size += sizeof(int)*2 * skill_count;

	if(pet_class == 5)
	{
		size += sizeof(_evo_prop);
	}

	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;			buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;				buf += sizeof(unsigned int);			//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);			//��Ʒ�Ķѵ�����
	*(int*)buf = 0;					buf += sizeof(int);				//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);				//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_PET_EGG_ESSENCE;		buf += sizeof(int);				//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;				buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;			buf += sizeof(int);				//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;
	*(int*) buf = petess->level_require;		buf += sizeof(int);
	*(int*) buf = petess->character_combo_id;	buf += sizeof(int);
	*(int*)	buf = ess->honor_point;			buf += sizeof(int);
	*(int*) buf = ess->id_pet;			buf += sizeof(int);
	*(int*) buf = 0;				buf += sizeof(int);
	*(int*)	buf = ess->id;				buf += sizeof(int);


	*(int*)	buf = pet_class;			buf += sizeof(int);
	*(short*)buf = ess->level;			buf += sizeof(short);
	*(unsigned short*)buf = 0;			buf += sizeof(unsigned short);
	*(int*)buf = ess->exp;				buf += sizeof(int);
	*(int*)buf = ess->skill_point;			buf += sizeof(int);
	*(short*)buf = 0;				buf += sizeof(short);
	*(short*)buf = skill_count;			buf += sizeof(short);
	memset(buf,0,16);				buf += 16;

	for(i = 0; i < skill_count; i ++)
	{
		*(int*)buf = skills[i].skill;		buf += sizeof(int);
		*(int*)buf = skills[i].level;		buf += sizeof(int);
	}
	if(pet_class == 5)
	{
		memset(buf,0,sizeof(_evo_prop));			buf += sizeof(_evo_prop);
	}
	ASSERT( (buf - (char*)*data) == (int)size);

	set_to_classid(DT_PET_EGG_ESSENCE, (item_data*)(*data), -1);
	return 0;
}


template <typename RAND_CLASS>
int generate_pet_food(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	PET_FOOD_ESSENCE * ess = (PET_FOOD_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_PET_FOOD_ESSENCE)	return -1;

	size = sizeof(item_data) + sizeof(_petfood_essense);

	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;			buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;				buf += sizeof(unsigned int);			//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);			//��Ʒ�Ķѵ�����
	*(int*)buf = 0;					buf += sizeof(int);				//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);				//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_PET_EGG_ESSENCE;		buf += sizeof(int);				//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;				buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;			buf += sizeof(int);				//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;
	*(int*) buf = ess->food_type;			buf += sizeof(int);
	*(int*) buf = ess->hornor;			buf += sizeof(int);

	ASSERT( (buf - (char*)*data) == (int)size);

	set_to_classid(DT_PET_FOOD_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_fireworks(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	FIREWORKS_ESSENCE * ess = (FIREWORKS_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_FIREWORKS_ESSENCE)	return -1;

	size = sizeof(item_data) ;
		
	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;				buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;						buf += sizeof(unsigned int);			//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);			//��Ʒ�Ķѵ�����
	*(int*)buf = 0;							buf += sizeof(int);				//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);				//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_FIREWORKS_ESSENCE;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;				buf += sizeof(int);				//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	set_to_classid(DT_FIREWORKS_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_tankcallin(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	WAR_TANKCALLIN_ESSENCE * ess = (WAR_TANKCALLIN_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_WAR_TANKCALLIN_ESSENCE)	return -1;

	size = sizeof(item_data) ;
		
	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;				buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;						buf += sizeof(unsigned int);			//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);			//��Ʒ�Ķѵ�����
	*(int*)buf = 0;							buf += sizeof(int);				//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);				//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_WAR_TANKCALLIN_ESSENCE;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;				buf += sizeof(int);				//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	set_to_classid(DT_WAR_TANKCALLIN_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_skillmatter(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	SKILLMATTER_ESSENCE* ess = (SKILLMATTER_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_SKILLMATTER_ESSENCE)	return -1;

	size = sizeof(item_data) + sizeof(int) * 3;
		
	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;				buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;						buf += sizeof(unsigned int);			//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);			//��Ʒ�Ķѵ�����
	*(int*)buf = 0;							buf += sizeof(int);				//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);				//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_SKILLMATTER_ESSENCE;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;				buf += sizeof(int);				//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;
	*(int*) buf = ess->level_required;	buf += sizeof(int);
	*(int*) buf = ess->id_skill;		buf += sizeof(int);
	*(int*) buf = ess->level_skill;		buf += sizeof(int);


	ASSERT( (buf - (char*)*data) == (int)size);
	set_to_classid(DT_SKILLMATTER_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_refine_ticket(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	REFINE_TICKET_ESSENCE * ess = (REFINE_TICKET_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_REFINE_TICKET_ESSENCE)	return -1;

	size = sizeof(item_data) ;
		
	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;				buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;						buf += sizeof(unsigned int);			//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);			//��Ʒ�Ķѵ�����
	*(int*)buf = 0;							buf += sizeof(int);				//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);				//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_REFINE_TICKET_ESSENCE;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;				buf += sizeof(int);				//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	set_to_classid(DT_REFINE_TICKET_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_bible(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	BIBLE_ESSENCE * ess = (BIBLE_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_BIBLE_ESSENCE)	return -1;

	size = sizeof(item_data) ;

	// ���addons
	char addon_buf[ELEMENTDATAMAN_MAX_NUM_ADDONS*sizeof(_addon)];
	unsigned int addon_num = 10;
	unsigned int addon_size = 0;
	addon_size = generate_equipment_addon_buffer_2(DT_BIBLE_ESSENCE, (int*)ess->id_addons, sizeof(int),addon_num, addon_buf,addon_num);
	size += sizeof(int);
	size += addon_size;

	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;			buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;				buf += sizeof(unsigned int);		//��Ʒ������
	*(unsigned int*)buf = 1;				buf += sizeof(unsigned int);		//��Ʒ�Ķѵ�����
	*(int*)buf = ELEMENTDATAMAN_EQUIP_MASK_BIBLE;	buf += sizeof(int);		//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);		//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_BIBLE_ESSENCE;			buf += sizeof(int);		//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);		//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);		//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);		//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;				buf += sizeof(int);		//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;			buf += sizeof(int);		//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;		buf += sizeof(unsigned int);		//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;		buf += sizeof(char *);		//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;
	
	// addons
	*(int*)buf = addon_num;				buf += sizeof(int);
	char * addon_sld = addon_buf;
	for(unsigned int i=0; i<addon_num; i++)
	{
		int id = *(int*)addon_sld;
		addon_sld += ((id & 0x6000)>>13)*sizeof(int)+sizeof(int);
	}
	memcpy(buf, addon_buf, addon_size);	
	buf += addon_size;
	ASSERT(buf - *data == (int)size);
	set_to_classid(DT_BIBLE_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_speaker(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	SPEAKER_ESSENCE * ess = (SPEAKER_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_SPEAKER_ESSENCE)	return -1;

	size = sizeof(item_data) + sizeof(int);

	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;			buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;				buf += sizeof(unsigned int);		//��Ʒ������
	*(unsigned int*)buf = 1;				buf += sizeof(unsigned int);		//��Ʒ�Ķѵ�����
	*(int*)buf = ELEMENTDATAMAN_EQUIP_MASK_SPEAKER;	buf += sizeof(int);		//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);		//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_SPEAKER_ESSENCE;		buf += sizeof(int);		//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);		//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);		//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);		//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;				buf += sizeof(int);		//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;			buf += sizeof(int);		//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;		buf += sizeof(unsigned int);		//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;		buf += sizeof(char *);		//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;
	
	// addons
	*(int*)buf = ess->id_icon_set;			buf += sizeof(int);
	ASSERT(buf - *data == (int)size);
	set_to_classid(DT_SPEAKER_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_hp_amulet(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	AUTOHP_ESSENCE * ess = (AUTOHP_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_AUTOHP_ESSENCE)	return -1;

	size = sizeof(item_data) + sizeof(int) + sizeof(float);

	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;			buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;				buf += sizeof(unsigned int);		//��Ʒ������
	*(unsigned int*)buf = 1;				buf += sizeof(unsigned int);		//��Ʒ�Ķѵ�����
	*(int*)buf = ELEMENTDATAMAN_EQUIP_MASK_AUTO_HP;	buf += sizeof(int);		//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);		//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_AUTOHP_ESSENCE;			buf += sizeof(int);		//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);		//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);		//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);		//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;				buf += sizeof(int);		//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;			buf += sizeof(int);		//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;		buf += sizeof(unsigned int);		//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;		buf += sizeof(char *);		//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;
	
	// addons
	*(int*)buf = ess->total_hp;			buf += sizeof(int);
	*(float*)buf = *(float*)&ess->trigger_amount;	buf += sizeof(float);
	ASSERT(buf - *data == (int)size);
	set_to_classid(DT_AUTOHP_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_mp_amulet(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	AUTOMP_ESSENCE * ess = (AUTOMP_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_AUTOMP_ESSENCE)	return -1;

	size = sizeof(item_data) + sizeof(int) + sizeof(float);

	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;			buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;				buf += sizeof(unsigned int);		//��Ʒ������
	*(unsigned int*)buf = 1;				buf += sizeof(unsigned int);		//��Ʒ�Ķѵ�����
	*(int*)buf = ELEMENTDATAMAN_EQUIP_MASK_AUTO_MP;	buf += sizeof(int);		//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);		//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_AUTOMP_ESSENCE;			buf += sizeof(int);		//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);		//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);		//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);		//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;				buf += sizeof(int);		//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;			buf += sizeof(int);		//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;		buf += sizeof(unsigned int);		//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;		buf += sizeof(char *);		//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;
	
	// addons
	*(int*)buf = ess->total_mp;			buf += sizeof(int);
	*(float*)buf = *(float*)&ess->trigger_amount;	buf += sizeof(float);
	ASSERT(buf - *data == (int)size);
	set_to_classid(DT_AUTOMP_ESSENCE, (item_data*)(*data), -1);
	return 0;
}


template <typename RAND_CLASS>
int generate_double_exp(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	DOUBLE_EXP_ESSENCE * ess = (DOUBLE_EXP_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_DOUBLE_EXP_ESSENCE)	return -1;

	size = sizeof(item_data) + sizeof(int);

	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;			buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;				buf += sizeof(unsigned int);		//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);		//��Ʒ�Ķѵ�����
	*(int*)buf = 0;				buf += sizeof(int);		//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);		//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_DOUBLE_EXP_ESSENCE;			buf += sizeof(int);		//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);		//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);		//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);		//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;				buf += sizeof(int);		//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;			buf += sizeof(int);		//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;		buf += sizeof(unsigned int);		//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;		buf += sizeof(char *);		//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;
	
	// addons
	*(int*)buf = ess->double_exp_time;		buf += sizeof(int);
	ASSERT(buf - *data == (int)size);
	set_to_classid(DT_DOUBLE_EXP_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_transmitscroll(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	TRANSMITSCROLL_ESSENCE * ess = (TRANSMITSCROLL_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_TRANSMITSCROLL_ESSENCE)	return -1;

	size = sizeof(item_data) ;
		
	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;				buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;						buf += sizeof(unsigned int);			//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);			//��Ʒ�Ķѵ�����
	*(int*)buf = 0;							buf += sizeof(int);				//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);				//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_TRANSMITSCROLL_ESSENCE;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;				buf += sizeof(int);				//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	set_to_classid(DT_TRANSMITSCROLL_ESSENCE, (item_data*)(*data), -1);
	return 0;
}


int generate_elf(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size) //lgc
{
	DATA_TYPE datatype;
	GOBLIN_ESSENCE * ess = (GOBLIN_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_GOBLIN_ESSENCE) return -1;

	//����item_data��С��С�����ʼ��װ���޼���?
	int default_skill_count = 0;
	if(ess->default_skill1 > 0) default_skill_count++;
	if(ess->default_skill2 > 0) default_skill_count++;
	if(ess->default_skill3 > 0) default_skill_count++;
	size = sizeof(item_data) + sizeof(_elf_item_content) + default_skill_count*sizeof(_elf_skill_data);
	
	*data = (char *)abase::fastalloc(size);
	char * buf = *data;
	
	//���item_data
	*(unsigned int *)buf = id;			buf += sizeof(unsigned int);//ģ��id
	*(unsigned int *)buf = 1;					buf += sizeof(unsigned int);
	*(unsigned int *)buf = ess->pile_num_max;	buf += sizeof(unsigned int);
	*(int *)buf = ELEMENTDATAMAN_EQUIP_MASK_ELF;			buf += sizeof(int);		//equip_mask
	*(int *)buf = ess->proc_type | 0x16;	buf += sizeof(int);		//proc_type
	*(int *)buf = DT_GOBLIN_ESSENCE;	buf += sizeof(int);		//classid
	if(ess->has_guid == 1)
	{
		int g1, g2;
		get_item_guid(id, g1, g2);
		*(int *)buf = g1;		buf += sizeof(int);
		*(int *)buf = g2;		buf += sizeof(int);
	}
	else
	{
		*(int *)buf = 0;		buf += sizeof(int);
		*(int *)buf = 0;		buf += sizeof(int);
	}
	*(int *)buf = ess->price;			buf += sizeof(int);
	*(int *)buf = 0;			buf += sizeof(int);
	unsigned int * content_length = (unsigned int *)buf;	buf += sizeof(unsigned int);
	char **	item_content = (char **)buf;		buf += sizeof(char *);
	*content_length = (char *)(*data) + size - buf;
	*item_content = buf;
	//essence
	*(unsigned int *)buf = 0;		buf += sizeof(unsigned int);		//exp
	*(short *)buf = 1;		buf += sizeof(short);	//level
	
	*(short *)buf = 0;		buf += sizeof(short);		//total_attri
	*(short *)buf = 0;		buf += sizeof(short);		//str
	*(short *)buf = 0;		buf += sizeof(short);		//agi
	*(short *)buf = 0;		buf += sizeof(short);		//vit
	*(short *)buf = 0;		buf += sizeof(short);		//eng
	
	*(short *)buf = 1;		buf += sizeof(short);		//total_genius
	*(short *)buf = 0;		buf += sizeof(short);		//g0
	*(short *)buf = 0;		buf += sizeof(short);		//g1
	*(short *)buf = 0;		buf += sizeof(short);		//g2
	*(short *)buf = 0;		buf += sizeof(short);		//g3
	*(short *)buf = 0;		buf += sizeof(short);		//g4
	
	*(short *)buf = 0;		buf += sizeof(short);		//refine_level
	*(int *)buf = 20000;		buf += sizeof(int);		//stamina
	*(int *)buf = 0;		buf += sizeof(int);			//status_value
	
	//memset(buf, 0, sizeof(struct _elf_essence)); 	buf += sizeof(struct _elf_essence);
	
	//equip
	*(int *)buf = 0;			buf += sizeof(int);
	//skill
	*(int *)buf = default_skill_count;			buf += sizeof(int);
	if(ess->default_skill1 > 0){
		*(unsigned short *)buf = ess->default_skill1;	buf += sizeof(unsigned short); 
		*(short *)buf = 1;								buf += sizeof(short);
	}
	if(ess->default_skill2 > 0){
		*(unsigned short *)buf = ess->default_skill2;	buf += sizeof(unsigned short); 
		*(short *)buf = 1;								buf += sizeof(short);
	}
	if(ess->default_skill3 > 0){
		*(unsigned short *)buf = ess->default_skill3;	buf += sizeof(unsigned short); 
		*(short *)buf = 1;								buf += sizeof(short);
	}
	
	set_to_classid(DT_GOBLIN_ESSENCE, (item_data*)(*data), -1);

	return 0;	
}

int generate_elf_equip(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size) //lgc
{
	DATA_TYPE datatype;
	GOBLIN_EQUIP_ESSENCE * ess = (GOBLIN_EQUIP_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_GOBLIN_EQUIP_ESSENCE) return -1;

	//����item_data��С��С����װ��item_body��������
	size = sizeof(item_data);
	
	*data = (char *)abase::fastalloc(size);
	char * buf = *data;
	
	//���item_data
	*(unsigned int *)buf = id;			buf += sizeof(unsigned int);//ģ��id
	*(unsigned int *)buf = 1;					buf += sizeof(unsigned int);
	*(unsigned int *)buf = ess->pile_num_max;	buf += sizeof(unsigned int);
	*(int *)buf = 0;					buf += sizeof(int);		//equip_mask
	*(int *)buf = ess->proc_type;		buf += sizeof(int);		//proc_type
	*(int *)buf = DT_GOBLIN_EQUIP_ESSENCE;	buf += sizeof(int);		//classid
	if(ess->has_guid == 1)
	{
		int g1, g2;
		get_item_guid(id, g1, g2);
		*(int *)buf = g1;		buf += sizeof(int);
		*(int *)buf = g2;		buf += sizeof(int);
	}
	else
	{
		*(int *)buf = 0;		buf += sizeof(int);
		*(int *)buf = 0;		buf += sizeof(int);
	}
	*(int *)buf = ess->price;			buf += sizeof(int);
	*(int *)buf = 0;			buf += sizeof(int);
	unsigned int * content_length = (unsigned int *)buf;	buf += sizeof(unsigned int);
	char **	item_content = (char **)buf;		buf += sizeof(char *);
	*content_length = (char *)(*data) + size - buf;
	*item_content = buf;
	
	set_to_classid(DT_GOBLIN_EQUIP_ESSENCE, (item_data*)(*data), -1);

	return 0;	
}

int generate_elf_exppill(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size) //lgc
{
	DATA_TYPE datatype;
	GOBLIN_EXPPILL_ESSENCE * ess = (GOBLIN_EXPPILL_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_GOBLIN_EXPPILL_ESSENCE) return -1;

	//����item_data��С��С���龭����item_body����int exp
	size = sizeof(item_data) + sizeof(_elf_exppill_essence);
	
	*data = (char *)abase::fastalloc(size);
	char * buf = *data;
	
	//���item_data
	*(unsigned int *)buf = id;			buf += sizeof(unsigned int);//ģ��id
	*(unsigned int *)buf = 1;					buf += sizeof(unsigned int);		//count
	*(unsigned int *)buf = ess->pile_num_max;	buf += sizeof(unsigned int);
	*(int *)buf = 0;					buf += sizeof(int);		//equip_mask
	*(int *)buf = ess->proc_type;		buf += sizeof(int);		//proc_type
	*(int *)buf = DT_GOBLIN_EXPPILL_ESSENCE;	buf += sizeof(int);		//classid
	if(ess->has_guid == 1)
	{
		int g1, g2;
		get_item_guid(id, g1, g2);
		*(int *)buf = g1;		buf += sizeof(int);
		*(int *)buf = g2;		buf += sizeof(int);
	}
	else
	{
		*(int *)buf = 0;		buf += sizeof(int);
		*(int *)buf = 0;		buf += sizeof(int);
	}
	*(int *)buf = ess->price;			buf += sizeof(int);
	*(int *)buf = 0;			buf += sizeof(int);		//����ʱ��
	unsigned int * content_length = (unsigned int *)buf;	buf += sizeof(unsigned int);
	char **	item_content = (char **)buf;		buf += sizeof(char *);
	*content_length = (char *)(*data) + size - buf;
	*item_content = buf;
	//exp
	*(unsigned int*)buf = ess->exp;				buf += sizeof(unsigned int);
	*(int*)buf = ess->level;					buf += sizeof(int);
	
	set_to_classid(DT_GOBLIN_EXPPILL_ESSENCE, (item_data*)(*data), -1);

	return 0;	
}


int generate_stallcard(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size)
{
	DATA_TYPE datatype;
	SELL_CERTIFICATE_ESSENCE * ess = (SELL_CERTIFICATE_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_SELL_CERTIFICATE_ESSENCE) return -1;

	//����item_data��С����̯ƾ֤item_body��������
	size = sizeof(item_data);
	
	*data = (char *)abase::fastalloc(size);
	char * buf = *data;
	
	//���item_data
	*(unsigned int *)buf = id;			buf += sizeof(unsigned int);//ģ��id
	*(unsigned int *)buf = 1;					buf += sizeof(unsigned int);
	*(unsigned int *)buf = ess->pile_num_max;	buf += sizeof(unsigned int);
	*(int *)buf = ELEMENTDATAMAN_EQUIP_MASK_STALLCARD;	buf += sizeof(int);		//equip_mask
	*(int *)buf = ess->proc_type;		buf += sizeof(int);		//proc_type
	*(int *)buf = DT_SELL_CERTIFICATE_ESSENCE;	buf += sizeof(int);		//classid
	if(ess->has_guid == 1)
	{
		int g1, g2;
		get_item_guid(id, g1, g2);
		*(int *)buf = g1;		buf += sizeof(int);
		*(int *)buf = g2;		buf += sizeof(int);
	}
	else
	{
		*(int *)buf = 0;		buf += sizeof(int);
		*(int *)buf = 0;		buf += sizeof(int);
	}
	*(int *)buf = ess->price;			buf += sizeof(int);
	*(int *)buf = 0;			buf += sizeof(int);
	unsigned int * content_length = (unsigned int *)buf;	buf += sizeof(unsigned int);
	char **	item_content = (char **)buf;		buf += sizeof(char *);
	*content_length = (char *)(*data) + size - buf;
	*item_content = buf;
	
	set_to_classid(DT_SELL_CERTIFICATE_ESSENCE, (item_data*)(*data), -1);

	return 0;	
}

int generate_skilltrigger2(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size)
{
	DATA_TYPE datatype;
	TARGET_ITEM_ESSENCE* ess = (TARGET_ITEM_ESSENCE*)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_TARGET_ITEM_ESSENCE) return -1;

	//����item_data��С��item_body��������
	size = sizeof(item_data);
	
	*data = (char *)abase::fastalloc(size);
	char * buf = *data;
	
	//���item_data
	*(unsigned int *)buf = id;			buf += sizeof(unsigned int);//ģ��id
	*(unsigned int *)buf = 1;					buf += sizeof(unsigned int);
	*(unsigned int *)buf = ess->pile_num_max;	buf += sizeof(unsigned int);
	*(int *)buf = 0;					buf += sizeof(int);		//equip_mask
	*(int *)buf = ess->proc_type;		buf += sizeof(int);		//proc_type
	*(int *)buf = DT_TARGET_ITEM_ESSENCE;	buf += sizeof(int);		//classid
	if(ess->has_guid == 1)
	{
		int g1, g2;
		get_item_guid(id, g1, g2);
		*(int *)buf = g1;		buf += sizeof(int);
		*(int *)buf = g2;		buf += sizeof(int);
	}
	else
	{
		*(int *)buf = 0;		buf += sizeof(int);
		*(int *)buf = 0;		buf += sizeof(int);
	}
	*(int *)buf = ess->price;			buf += sizeof(int);
	*(int *)buf = 0;					buf += sizeof(int);
	unsigned int * content_length = (unsigned int *)buf;	buf += sizeof(unsigned int);
	char **	item_content = (char **)buf;		buf += sizeof(char *);
	*content_length = (char *)(*data) + size - buf;
	*item_content = buf;
	
	set_to_classid(DT_TARGET_ITEM_ESSENCE, (item_data*)(*data), -1);

	return 0;	
}

int generate_queryotherproperty(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size)
{
	DATA_TYPE datatype;
	LOOK_INFO_ESSENCE* ess = (LOOK_INFO_ESSENCE*)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_LOOK_INFO_ESSENCE) return -1;

	//����item_data��С��item_body��������
	size = sizeof(item_data);
	
	*data = (char *)abase::fastalloc(size);
	char * buf = *data;
	
	//���item_data
	*(unsigned int *)buf = id;			buf += sizeof(unsigned int);//ģ��id
	*(unsigned int *)buf = 1;					buf += sizeof(unsigned int);
	*(unsigned int *)buf = ess->pile_num_max;	buf += sizeof(unsigned int);
	*(int *)buf = 0;					buf += sizeof(int);		//equip_mask
	*(int *)buf = ess->proc_type;		buf += sizeof(int);		//proc_type
	*(int *)buf = DT_LOOK_INFO_ESSENCE;	buf += sizeof(int);		//classid
	if(ess->has_guid == 1)
	{
		int g1, g2;
		get_item_guid(id, g1, g2);
		*(int *)buf = g1;		buf += sizeof(int);
		*(int *)buf = g2;		buf += sizeof(int);
	}
	else
	{
		*(int *)buf = 0;		buf += sizeof(int);
		*(int *)buf = 0;		buf += sizeof(int);
	}
	*(int *)buf = ess->price;			buf += sizeof(int);
	*(int *)buf = 0;					buf += sizeof(int);
	unsigned int * content_length = (unsigned int *)buf;	buf += sizeof(unsigned int);
	char **	item_content = (char **)buf;		buf += sizeof(char *);
	*content_length = (char *)(*data) + size - buf;
	*item_content = buf;
	
	set_to_classid(DT_LOOK_INFO_ESSENCE, (item_data*)(*data), -1);

	return 0;	
}

int generate_incskillability(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size)
{
	DATA_TYPE datatype;
	INC_SKILL_ABILITY_ESSENCE* ess = (INC_SKILL_ABILITY_ESSENCE*)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_INC_SKILL_ABILITY_ESSENCE) return -1;

	//����item_data��С��item_body��������
	size = sizeof(item_data);
	
	*data = (char *)abase::fastalloc(size);
	char * buf = *data;
	
	//���item_data
	*(unsigned int *)buf = id;			buf += sizeof(unsigned int);//ģ��id
	*(unsigned int *)buf = 1;					buf += sizeof(unsigned int);
	*(unsigned int *)buf = ess->pile_num_max;	buf += sizeof(unsigned int);
	*(int *)buf = 0;					buf += sizeof(int);		//equip_mask
	*(int *)buf = ess->proc_type;		buf += sizeof(int);		//proc_type
	*(int *)buf = DT_INC_SKILL_ABILITY_ESSENCE;	buf += sizeof(int);		//classid
	if(ess->has_guid == 1)
	{
		int g1, g2;
		get_item_guid(id, g1, g2);
		*(int *)buf = g1;		buf += sizeof(int);
		*(int *)buf = g2;		buf += sizeof(int);
	}
	else
	{
		*(int *)buf = 0;		buf += sizeof(int);
		*(int *)buf = 0;		buf += sizeof(int);
	}
	*(int *)buf = ess->price;			buf += sizeof(int);
	*(int *)buf = 0;					buf += sizeof(int);
	unsigned int * content_length = (unsigned int *)buf;	buf += sizeof(unsigned int);
	char **	item_content = (char **)buf;		buf += sizeof(char *);
	*content_length = (char *)(*data) + size - buf;
	*item_content = buf;
	
	set_to_classid(DT_INC_SKILL_ABILITY_ESSENCE, (item_data*)(*data), -1);

	return 0;	
}

int generate_wedding_bookcard(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size)
{
	DATA_TYPE datatype;
	WEDDING_BOOKCARD_ESSENCE * ess = (WEDDING_BOOKCARD_ESSENCE*)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_WEDDING_BOOKCARD_ESSENCE) return -1;

	//����item_data��С
	size = sizeof(item_data) + sizeof(_wedding_bookcard_essence);
	
	*data = (char *)abase::fastalloc(size);
	char * buf = *data;
	
	//���item_data
	*(unsigned int *)buf = id;			buf += sizeof(unsigned int);//ģ��id
	*(unsigned int *)buf = 1;					buf += sizeof(unsigned int);		//count
	*(unsigned int *)buf = ess->pile_num_max;	buf += sizeof(unsigned int);
	*(int *)buf = 0;					buf += sizeof(int);		//equip_mask
	*(int *)buf = ess->proc_type;		buf += sizeof(int);		//proc_type
	*(int *)buf = DT_WEDDING_BOOKCARD_ESSENCE;	buf += sizeof(int);		//classid
	if(ess->has_guid == 1)
	{
		int g1, g2;
		get_item_guid(id, g1, g2);
		*(int *)buf = g1;		buf += sizeof(int);
		*(int *)buf = g2;		buf += sizeof(int);
	}
	else
	{
		*(int *)buf = 0;		buf += sizeof(int);
		*(int *)buf = 0;		buf += sizeof(int);
	}
	*(int *)buf = ess->price;			buf += sizeof(int);
	*(int *)buf = 0;			buf += sizeof(int);		//����ʱ��
	unsigned int * content_length = (unsigned int *)buf;	buf += sizeof(unsigned int);
	char **	item_content = (char **)buf;		buf += sizeof(char *);
	*content_length = (char *)(*data) + size - buf;
	*item_content = buf;
	//
	*(int*)buf = ess->year;				buf += sizeof(int);
	*(int*)buf = ess->month;			buf += sizeof(int);
	*(int*)buf = ess->day;				buf += sizeof(int);
	
	set_to_classid(DT_WEDDING_BOOKCARD_ESSENCE, (item_data*)(*data), -1);

	return 0;	
}

int generate_wedding_invitecard(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size)
{
	DATA_TYPE datatype;
	WEDDING_INVITECARD_ESSENCE* ess = (WEDDING_INVITECARD_ESSENCE*)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_WEDDING_INVITECARD_ESSENCE) return -1;

	//����item_data��С
	size = sizeof(item_data) + sizeof(_wedding_invitecard_essence);
	
	*data = (char *)abase::fastalloc(size);
	char * buf = *data;
	
	//���item_data
	*(unsigned int *)buf = id;			buf += sizeof(unsigned int);//ģ��id
	*(unsigned int *)buf = 1;					buf += sizeof(unsigned int);		//count
	*(unsigned int *)buf = ess->pile_num_max;	buf += sizeof(unsigned int);
	*(int *)buf = 0;					buf += sizeof(int);		//equip_mask
	*(int *)buf = ess->proc_type;		buf += sizeof(int);		//proc_type
	*(int *)buf = DT_WEDDING_INVITECARD_ESSENCE;	buf += sizeof(int);		//classid
	if(ess->has_guid == 1)
	{
		int g1, g2;
		get_item_guid(id, g1, g2);
		*(int *)buf = g1;		buf += sizeof(int);
		*(int *)buf = g2;		buf += sizeof(int);
	}
	else
	{
		*(int *)buf = 0;		buf += sizeof(int);
		*(int *)buf = 0;		buf += sizeof(int);
	}
	*(int *)buf = ess->price;			buf += sizeof(int);
	*(int *)buf = 0;			buf += sizeof(int);		//����ʱ��
	unsigned int * content_length = (unsigned int *)buf;	buf += sizeof(unsigned int);
	char **	item_content = (char **)buf;		buf += sizeof(char *);
	*content_length = (char *)(*data) + size - buf;
	*item_content = buf;
	//
	*(int*)buf = 0;				buf += sizeof(int);	//start time
	*(int*)buf = 0;				buf += sizeof(int);	//end time
	*(int*)buf = 0;				buf += sizeof(int);	//groom
	*(int*)buf = 0;				buf += sizeof(int);	//bride
	*(int*)buf = 0;				buf += sizeof(int); //scene
	*(int*)buf = 0;				buf += sizeof(int);	//invitee
	
	set_to_classid(DT_WEDDING_INVITECARD_ESSENCE, (item_data*)(*data), -1);

	return 0;	
}

int generate_sharpener(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size)
{
	DATA_TYPE datatype;
	SHARPENER_ESSENCE* ess = (SHARPENER_ESSENCE*)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_SHARPENER_ESSENCE) return -1;

	//����item_data��С��item_body��������
	size = sizeof(item_data);
	
	*data = (char *)abase::fastalloc(size);
	char * buf = *data;
	
	//���item_data
	*(unsigned int *)buf = id;			buf += sizeof(unsigned int);//ģ��id
	*(unsigned int *)buf = 1;					buf += sizeof(unsigned int);		//count
	*(unsigned int *)buf = ess->pile_num_max;	buf += sizeof(unsigned int);
	*(int *)buf = 0;					buf += sizeof(int);		//equip_mask
	*(int *)buf = ess->proc_type;		buf += sizeof(int);		//proc_type
	*(int *)buf = DT_SHARPENER_ESSENCE;	buf += sizeof(int);		//classid
	if(ess->has_guid == 1)
	{
		int g1, g2;
		get_item_guid(id, g1, g2);
		*(int *)buf = g1;		buf += sizeof(int);
		*(int *)buf = g2;		buf += sizeof(int);
	}
	else
	{
		*(int *)buf = 0;		buf += sizeof(int);
		*(int *)buf = 0;		buf += sizeof(int);
	}
	*(int *)buf = ess->price;			buf += sizeof(int);
	*(int *)buf = 0;			buf += sizeof(int);		//����ʱ��
	unsigned int * content_length = (unsigned int *)buf;	buf += sizeof(unsigned int);
	char **	item_content = (char **)buf;		buf += sizeof(char *);
	*content_length = 0;
	*item_content = buf;
	//
	set_to_classid(DT_SHARPENER_ESSENCE, (item_data*)(*data), -1);

	return 0;	
}

int generate_factionmaterial(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size)
{
	DATA_TYPE datatype;
	FACTION_MATERIAL_ESSENCE* ess = (FACTION_MATERIAL_ESSENCE*)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_FACTION_MATERIAL_ESSENCE) return -1;

	//����item_data��С��item_body��������
	size = sizeof(item_data);
	
	*data = (char *)abase::fastalloc(size);
	char * buf = *data;
	
	//���item_data
	*(unsigned int *)buf = id;			buf += sizeof(unsigned int);//ģ��id
	*(unsigned int *)buf = 1;					buf += sizeof(unsigned int);		//count
	*(unsigned int *)buf = ess->pile_num_max;	buf += sizeof(unsigned int);
	*(int *)buf = 0;					buf += sizeof(int);		//equip_mask
	*(int *)buf = ess->proc_type;		buf += sizeof(int);		//proc_type
	*(int *)buf = DT_FACTION_MATERIAL_ESSENCE;	buf += sizeof(int);		//classid
	if(ess->has_guid == 1)
	{
		int g1, g2;
		get_item_guid(id, g1, g2);
		*(int *)buf = g1;		buf += sizeof(int);
		*(int *)buf = g2;		buf += sizeof(int);
	}
	else
	{
		*(int *)buf = 0;		buf += sizeof(int);
		*(int *)buf = 0;		buf += sizeof(int);
	}
	*(int *)buf = ess->price;			buf += sizeof(int);
	*(int *)buf = 0;			buf += sizeof(int);		//����ʱ��
	unsigned int * content_length = (unsigned int *)buf;	buf += sizeof(unsigned int);
	char **	item_content = (char **)buf;		buf += sizeof(char *);
	*content_length = 0;
	*item_content = buf;
	//
	set_to_classid(DT_FACTION_MATERIAL_ESSENCE, (item_data*)(*data), -1);

	return 0;	
}

int generate_congregate(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size)
{
	DATA_TYPE datatype;
	CONGREGATE_ESSENCE* ess = (CONGREGATE_ESSENCE*)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_CONGREGATE_ESSENCE) return -1;

	//����item_data��С��item_body��������
	size = sizeof(item_data);
	
	*data = (char *)abase::fastalloc(size);
	char * buf = *data;
	
	//���item_data
	*(unsigned int *)buf = id;			buf += sizeof(unsigned int);//ģ��id
	*(unsigned int *)buf = 1;					buf += sizeof(unsigned int);		//count
	*(unsigned int *)buf = ess->pile_num_max;	buf += sizeof(unsigned int);
	*(int *)buf = 0;					buf += sizeof(int);		//equip_mask
	*(int *)buf = ess->proc_type;		buf += sizeof(int);		//proc_type
	*(int *)buf = DT_CONGREGATE_ESSENCE;	buf += sizeof(int);		//classid
	if(ess->has_guid == 1)
	{
		int g1, g2;
		get_item_guid(id, g1, g2);
		*(int *)buf = g1;		buf += sizeof(int);
		*(int *)buf = g2;		buf += sizeof(int);
	}
	else
	{
		*(int *)buf = 0;		buf += sizeof(int);
		*(int *)buf = 0;		buf += sizeof(int);
	}
	*(int *)buf = ess->price;			buf += sizeof(int);
	*(int *)buf = 0;			buf += sizeof(int);		//����ʱ��
	unsigned int * content_length = (unsigned int *)buf;	buf += sizeof(unsigned int);
	char **	item_content = (char **)buf;		buf += sizeof(char *);
	*content_length = 0;
	*item_content = buf;
	//
	set_to_classid(DT_CONGREGATE_ESSENCE, (item_data*)(*data), -1);

	return 0;	
}
int generate_force_ticket(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size)
{
	DATA_TYPE datatype;
	FORCE_TOKEN_ESSENCE* ess = (FORCE_TOKEN_ESSENCE*)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_FORCE_TOKEN_ESSENCE) return -1;

	//����item_data��С
	size = sizeof(item_data) + sizeof(_force_ticket_essence);
	
	*data = (char *)abase::fastalloc(size);
	char * buf = *data;
	
	//���item_data
	*(unsigned int *)buf = id;			buf += sizeof(unsigned int);//ģ��id
	*(unsigned int *)buf = 1;					buf += sizeof(unsigned int);		//count
	*(unsigned int *)buf = ess->pile_num_max;	buf += sizeof(unsigned int);
	*(int *)buf = ELEMENTDATAMAN_EQUIP_MASK_FORCE_TICKET;	buf += sizeof(int);		//equip_mask
	*(int *)buf = ess->proc_type;		buf += sizeof(int);		//proc_type
	*(int *)buf = DT_FORCE_TOKEN_ESSENCE;	buf += sizeof(int);		//classid
	if(ess->has_guid == 1)
	{
		int g1, g2;
		get_item_guid(id, g1, g2);
		*(int *)buf = g1;		buf += sizeof(int);
		*(int *)buf = g2;		buf += sizeof(int);
	}
	else
	{
		*(int *)buf = 0;		buf += sizeof(int);
		*(int *)buf = 0;		buf += sizeof(int);
	}
	*(int *)buf = ess->price;			buf += sizeof(int);
	*(int *)buf = 0;			buf += sizeof(int);		//����ʱ��
	unsigned int * content_length = (unsigned int *)buf;	buf += sizeof(unsigned int);
	char **	item_content = (char **)buf;		buf += sizeof(char *);
	*content_length = (char *)(*data) + size - buf;
	*item_content = buf;
	//
	*(int*)buf = ess->require_force;				buf += sizeof(int);
	*(int*)buf = ess->reputation_add;				buf += sizeof(int);
	*(int*)buf = ess->reputation_increase_ratio;	buf += sizeof(int);
	
	set_to_classid(DT_FORCE_TOKEN_ESSENCE, (item_data*)(*data), -1);

	return 0;	
}
int generate_dynskillequip(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size)
{
	DATA_TYPE datatype;
	DYNSKILLEQUIP_ESSENCE* ess = (DYNSKILLEQUIP_ESSENCE*)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_DYNSKILLEQUIP_ESSENCE) return -1;

	//����item_data��С��item_body��������
	size = sizeof(item_data);
	
	*data = (char *)abase::fastalloc(size);
	char * buf = *data;
	
	//���item_data
	*(unsigned int *)buf = id;			buf += sizeof(unsigned int);//ģ��id
	*(unsigned int *)buf = 1;					buf += sizeof(unsigned int);		//count
	*(unsigned int *)buf = ess->pile_num_max;	buf += sizeof(unsigned int);
	*(int *)buf = ELEMENTDATAMAN_EQUIP_MASK_DYNSKILL_ALL;					buf += sizeof(int);		//equip_mask
	*(int *)buf = ess->proc_type;		buf += sizeof(int);		//proc_type
	*(int *)buf = DT_DYNSKILLEQUIP_ESSENCE;	buf += sizeof(int);		//classid
	if(ess->has_guid == 1)
	{
		int g1, g2;
		get_item_guid(id, g1, g2);
		*(int *)buf = g1;		buf += sizeof(int);
		*(int *)buf = g2;		buf += sizeof(int);
	}
	else
	{
		*(int *)buf = 0;		buf += sizeof(int);
		*(int *)buf = 0;		buf += sizeof(int);
	}
	*(int *)buf = ess->price;			buf += sizeof(int);
	*(int *)buf = 0;			buf += sizeof(int);		//����ʱ��
	unsigned int * content_length = (unsigned int *)buf;	buf += sizeof(unsigned int);
	char **	item_content = (char **)buf;		buf += sizeof(char *);
	*content_length = 0;
	*item_content = buf;
	//
	set_to_classid(DT_DYNSKILLEQUIP_ESSENCE, (item_data*)(*data), -1);

	return 0;	
}
int generate_moneyconvertibleitem(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size)
{
	DATA_TYPE datatype;
	MONEY_CONVERTIBLE_ESSENCE * ess = (MONEY_CONVERTIBLE_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_MONEY_CONVERTIBLE_ESSENCE)	return -1;

	size = sizeof(item_data);

	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;				buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;						buf += sizeof(unsigned int);			//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);			//��Ʒ�Ķѵ�����
	*(int*)buf = 0;							buf += sizeof(int);				//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);				//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_MONEY_CONVERTIBLE_ESSENCE;		buf += sizeof(int);				//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;				buf += sizeof(int);				//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	set_to_classid(DT_MONEY_CONVERTIBLE_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_generalcard(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	POKER_ESSENCE * ess = (POKER_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_POKER_ESSENCE)	return -1;
	POKER_SUB_TYPE * sub_type = (POKER_SUB_TYPE *)get_data_ptr(ess->id_sub_type, ID_SPACE_ESSENCE, datatype);
	if(sub_type == NULL || datatype != DT_POKER_SUB_TYPE) return -1;

	size = sizeof(item_data) + sizeof(_generalcard_essence);

	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;				buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;						buf += sizeof(unsigned int);			//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);			//��Ʒ�Ķѵ�����
	*(int*)buf = equip_mask_64_to_32(sub_type->equip_mask);		buf += sizeof(int);				//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);				//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_POKER_ESSENCE;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;				buf += sizeof(int);				//��Ʒ�ļ۸�
	*(int*)buf = 0;							buf += sizeof(int);   			//����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;
	//
	*(int*)buf = sub_type->type;				buf += sizeof(int);
	*(int*)buf = ess->rank;						buf += sizeof(int);
	*(int*)buf = ess->require_level;			buf += sizeof(int);
	int require_leadership = element_data::RandNormal(ess->require_control_point[0],ess->require_control_point[1],cls,element_data::LOWER_TREND);
	*(int*)buf = require_leadership;			buf += sizeof(int);
	*(int*)buf = ess->max_level;				buf += sizeof(int);
	*(int*)buf = 1;								buf += sizeof(int);
	*(int*)buf = 0;								buf += sizeof(int);
	*(int*)buf = 0;								buf += sizeof(int);

	set_to_classid(DT_POKER_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

int generate_generalcard_dice(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size)
{
	DATA_TYPE datatype;
	POKER_DICE_ESSENCE * ess = (POKER_DICE_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_POKER_DICE_ESSENCE)	return -1;

	size = sizeof(item_data);

	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;				buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;						buf += sizeof(unsigned int);			//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);			//��Ʒ�Ķѵ�����
	*(int*)buf = 0;							buf += sizeof(int);				//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);				//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_POKER_DICE_ESSENCE;		buf += sizeof(int);				//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;				buf += sizeof(int);				//��Ʒ�ļ۸�
	*(int*)buf = 0;							buf += sizeof(int);   			//����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	set_to_classid(DT_POKER_DICE_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

int generate_soul(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size)
{
	DATA_TYPE datatype;
	MONSTER_SPIRIT_ESSENCE * ess = (MONSTER_SPIRIT_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_MONSTER_SPIRIT_ESSENCE)	return -1;

	size = sizeof(item_data);
	
	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);
	
	*(unsigned int*)buf = id;				buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;						buf += sizeof(unsigned int);			//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);			//��Ʒ�Ķѵ�����
	*(int*)buf = 0;							buf += sizeof(int);				//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);				//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_MONSTER_SPIRIT_ESSENCE;	buf += sizeof(int);				//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;				buf += sizeof(int);				//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;
	
	set_to_classid(DT_MONSTER_SPIRIT_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

int generate_astrolabe(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size)
{
	DATA_TYPE datatype;
	ASTROLABE_ESSENCE * ess = (ASTROLABE_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_ASTROLABE_ESSENCE)	return -1;

	size = sizeof(item_data) + sizeof(_astrolabe_essence) + sizeof(int);
	
	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);
	
	*(unsigned int*)buf = id;				buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;						buf += sizeof(unsigned int);			//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);			//��Ʒ�Ķѵ�����
	*(int*)buf = equip_mask_64_to_32(ELEMENTDATAMAN_EQUIP_MASK_ASTROLABE);							buf += sizeof(int);				//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);				//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_ASTROLABE_ESSENCE;		buf += sizeof(int);				//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;				buf += sizeof(int);				//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;
	// ���̰װ�
	memset(buf,0,sizeof(_astrolabe_essence) + sizeof(int));

	set_to_classid(DT_ASTROLABE_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

int generate_occup_package(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size)
{
	DATA_TYPE datatype;
	ITEM_PACKAGE_BY_PROFESSION_ESSENCE * ess = (ITEM_PACKAGE_BY_PROFESSION_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_ITEM_PACKAGE_BY_PROFESSION_ESSENCE)	return -1;

	size = sizeof(item_data);

	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;				buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;						buf += sizeof(unsigned int);			//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);			//��Ʒ�Ķѵ�����
	*(int*)buf = 0;							buf += sizeof(int);				//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);				//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_ITEM_PACKAGE_BY_PROFESSION_ESSENCE;		buf += sizeof(int);				//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;				buf += sizeof(int);				//��Ʒ�ļ۸�
	*(int*)buf = 0;							buf += sizeof(int);   			//����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	set_to_classid(DT_ITEM_PACKAGE_BY_PROFESSION_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

int generate_shoptoken(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size)
{
	DATA_TYPE datatype;
	SHOP_TOKEN_ESSENCE * ess = (SHOP_TOKEN_ESSENCE*)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_SHOP_TOKEN_ESSENCE)	return -1;

	size = sizeof(item_data);

	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;				buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;						buf += sizeof(unsigned int);			//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);			//��Ʒ�Ķѵ�����
	*(int*)buf = 0;							buf += sizeof(int);				//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);				//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_SHOP_TOKEN_ESSENCE;		buf += sizeof(int);				//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;				buf += sizeof(int);				//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	set_to_classid(DT_SHOP_TOKEN_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

int generate_universal_token(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size)
{
	DATA_TYPE datatype;
	UNIVERSAL_TOKEN_ESSENCE * ess = (UNIVERSAL_TOKEN_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_UNIVERSAL_TOKEN_ESSENCE)	return -1;

	size = sizeof(item_data);

	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;				buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;						buf += sizeof(unsigned int);			//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);			//��Ʒ�Ķѵ�����
	*(int*)buf = 0;							buf += sizeof(int);				//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);				//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_UNIVERSAL_TOKEN_ESSENCE;		buf += sizeof(int);				//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;				buf += sizeof(int);				//��Ʒ�ļ۸�
	*(int*)buf = 0;							buf += sizeof(int);   			//����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	set_to_classid(DT_UNIVERSAL_TOKEN_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_fireworks2(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	FIREWORKS2_ESSENCE * ess = (FIREWORKS2_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_FIREWORKS2_ESSENCE)	return -1;

	size = sizeof(item_data);
		
	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;				buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;						buf += sizeof(unsigned int);			//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);			//��Ʒ�Ķѵ�����
	*(int*)buf = 0;							buf += sizeof(int);				//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);				//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_FIREWORKS2_ESSENCE;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;				buf += sizeof(int);				//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	set_to_classid(DT_FIREWORKS2_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_fixpositiontransmit(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	FIX_POSITION_TRANSMIT_ESSENCE * ess = (FIX_POSITION_TRANSMIT_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_FIX_POSITION_TRANSMIT_ESSENCE)	return -1;

	size = sizeof(item_data);
		
	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;				buf += sizeof(unsigned int);	//��Ʒ��ģ��ID
	*(unsigned int*)buf = 1;						buf += sizeof(unsigned int);			//��Ʒ������
	*(unsigned int*)buf = ess->pile_num_max;		buf += sizeof(unsigned int);			//��Ʒ�Ķѵ�����
	*(int*)buf = 0;							buf += sizeof(int);				//��Ʒ�Ŀ�װ����־
	*(int*)buf = ess->proc_type;			buf += sizeof(int);				//��Ʒ�Ĵ�����ʽ
	*(int*)buf = DT_FIX_POSITION_TRANSMIT_ESSENCE;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	
	if(ess->has_guid == 1){
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = g2;			buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	else{
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
		*(int*)buf = 0;						buf += sizeof(int);				//��Ʒ��Ӧ�����ID	guid
	}
	*(int*)buf = ess->price;				buf += sizeof(int);				//��Ʒ�ļ۸�
	*(int*)buf = 0;	buf += sizeof(int);   //����ʱ��
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	set_to_classid(DT_FIX_POSITION_TRANSMIT_ESSENCE, (item_data*)(*data), -1);
	return 0;
}


// Novas Fulano 

template <typename RAND_CLASS>
int generate_easy_produce_item(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	EASY_PRODUCE_ITEM_ESSENCE * ess = (EASY_PRODUCE_ITEM_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_EASY_PRODUCE_ITEM_ESSENCE)	return -1;

	size = sizeof(item_data);
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;					buf += sizeof(unsigned int); //ид
	*(unsigned int*)buf = 1;					buf += sizeof(unsigned int); //количество	
	*(unsigned int*)buf = ess->pile_num_max;	buf += sizeof(unsigned int); //макс количество

	*(int*)buf = 0;								buf += sizeof(unsigned int); //октет
	*(int*)buf = ess->proc_type;				buf += sizeof(unsigned int); //привязка
	
	*(int*)buf = DT_EASY_PRODUCE_ITEM_ESSENCE;	buf += sizeof(unsigned int); //номер листа	
	
	if(ess->has_guid == 1) // генерировать гуид
	{
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				
		*(int*)buf = g2;			buf += sizeof(int);				
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);				
		*(int*)buf = 0;				buf += sizeof(int);				
	}
	
	*(int*)buf = ess->price;		buf += sizeof(int);				//цена
	*(int*)buf = 0;					buf += sizeof(int);   			//маска
	
	
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	set_to_classid(DT_EASY_PRODUCE_ITEM_ESSENCE, (item_data*)(*data), -1);
	return 0;
}
	

template <typename RAND_CLASS>
int generate_fix_monster_item(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	FIX_MONSTER_ITEM_ESSENCE * ess = (FIX_MONSTER_ITEM_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_FIX_MONSTER_ITEM_ESSENCE)	return -1;

	size = sizeof(item_data);
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;					buf += sizeof(unsigned int); //Id
	*(unsigned int*)buf = 1;					buf += sizeof(unsigned int); //número	
	*(unsigned int*)buf = ess->pile_num_max;	buf += sizeof(unsigned int); //número máximo

	*(int*)buf = 0;								buf += sizeof(unsigned int); //octeto
	*(int*)buf = ess->proc_type;				buf += sizeof(unsigned int); //vinculante
	
	*(int*)buf = DT_FIX_MONSTER_ITEM_ESSENCE;	buf += sizeof(unsigned int); //número da folha	
	
	if(ess->has_guid == 1) // gerar uma diretriz
	{
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				
		*(int*)buf = g2;			buf += sizeof(int);				
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);				
		*(int*)buf = 0;				buf += sizeof(int);				
	}
	
	*(int*)buf = ess->price;		buf += sizeof(int);				//preço
	*(int*)buf = 0;					buf += sizeof(int);   			//máscara
	
	
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	set_to_classid(DT_FIX_MONSTER_ITEM_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_bible_refine_ticket_item(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	BIBLE_REFINE_TICKET_ESSENCE * ess = (BIBLE_REFINE_TICKET_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_BIBLE_REFINE_TICKET_ESSENCE)	return -1;

	size = sizeof(item_data);
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;					buf += sizeof(unsigned int); //Id
	*(unsigned int*)buf = 1;					buf += sizeof(unsigned int); //número	
	*(unsigned int*)buf = ess->pile_num_max;	buf += sizeof(unsigned int); //número máximo

	*(int*)buf = 0;								buf += sizeof(unsigned int); //octeto
	*(int*)buf = ess->proc_type;				buf += sizeof(unsigned int); //vinculante
	
	*(int*)buf = DT_BIBLE_REFINE_TICKET_ESSENCE;	buf += sizeof(unsigned int); //número da folha	
	
	if(ess->has_guid == 1) // gerar uma diretriz
	{
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				
		*(int*)buf = g2;			buf += sizeof(int);				
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);				
		*(int*)buf = 0;				buf += sizeof(int);				
	}
	
	*(int*)buf = ess->price;		buf += sizeof(int);				//preço
	*(int*)buf = 0;					buf += sizeof(int);   			//máscara
	
	
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	set_to_classid(DT_BIBLE_REFINE_TICKET_ESSENCE, (item_data*)(*data), -1);
	return 0;
}


template <typename RAND_CLASS>
int generate_used_for_area_item(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	ITEM_USED_FOR_AREA_ESSENCE * ess = (ITEM_USED_FOR_AREA_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_ITEM_USED_FOR_AREA_ESSENCE)	return -1;

	size = sizeof(item_data);
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;					buf += sizeof(unsigned int); //Id
	*(unsigned int*)buf = 1;					buf += sizeof(unsigned int); //número	
	*(unsigned int*)buf = ess->pile_num_max;	buf += sizeof(unsigned int); //número máximo

	*(int*)buf = 0;								buf += sizeof(unsigned int); //octeto
	*(int*)buf = ess->proc_type;				buf += sizeof(unsigned int); //vinculante
	
	*(int*)buf = DT_ITEM_USED_FOR_AREA_ESSENCE;	buf += sizeof(unsigned int); //número da folha	
	
	if(ess->has_guid == 1) // gerar uma diretriz
	{
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				
		*(int*)buf = g2;			buf += sizeof(int);				
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);				
		*(int*)buf = 0;				buf += sizeof(int);				
	}
	
	*(int*)buf = ess->price;		buf += sizeof(int);				//preço
	*(int*)buf = 0;					buf += sizeof(int);   			//máscara
	
	
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	set_to_classid(DT_ITEM_USED_FOR_AREA_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_capture_item(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	CAPTURE_ITEM_ESSENCE * ess = (CAPTURE_ITEM_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_CAPTURE_ITEM_ESSENCE)	return -1;

	size = sizeof(item_data);
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;					buf += sizeof(unsigned int); //Id
	*(unsigned int*)buf = 1;					buf += sizeof(unsigned int); //número	
	*(unsigned int*)buf = ess->pile_num_max;	buf += sizeof(unsigned int); //número máximo

	*(int*)buf = 0;								buf += sizeof(unsigned int); //octeto
	*(int*)buf = ess->proc_type;				buf += sizeof(unsigned int); //vinculante
	
	*(int*)buf = DT_CAPTURE_ITEM_ESSENCE;	buf += sizeof(unsigned int); //número da folha	
	
	if(ess->has_guid == 1) // gerar uma diretriz
	{
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				
		*(int*)buf = g2;			buf += sizeof(int);				
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);				
		*(int*)buf = 0;				buf += sizeof(int);				
	}
	
	*(int*)buf = ess->price;		buf += sizeof(int);				//preço
	*(int*)buf = 0;					buf += sizeof(int);   			//máscara
	
	
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	set_to_classid(DT_CAPTURE_ITEM_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_qihun_item(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls,element_data::GEN_ADDON_MODE normal_addon ,const void * tag,unsigned int tag_size, int class_pos, int * sa_list = NULL)
{
	DATA_TYPE datatype;
	QIHUN_ESSENCE * ess = (QIHUN_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_QIHUN_ESSENCE)	return -1;

	// Addons e Cristal
	unsigned int addon_size = 0;

	unsigned int addon_num = element_data::RandSelect(&(ess->probability_addon_num0),sizeof(float), 6,cls,element_data::LOWER_TREND);
	unsigned int moon_num = element_data::RandSelect(&(ess->probability_moon_phase_level0),sizeof(float), 6,cls,element_data::LOWER_TREND);


	size = sizeof(item_data) + sizeof(_qihun_essence);

	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;					buf += sizeof(unsigned int); //Id
	*(unsigned int*)buf = 1;					buf += sizeof(unsigned int); //número	
	*(unsigned int*)buf = ess->pile_num_max;	buf += sizeof(unsigned int); //número máximo

	*(int*)buf = 0;								buf += sizeof(unsigned int); //octeto
	*(int*)buf = ess->proc_type;				buf += sizeof(unsigned int); //vinculante
	
	*(int*)buf = DT_QIHUN_ESSENCE;	buf += sizeof(unsigned int); //número da folha	
	
	if(ess->has_guid == 1) // gerar uma diretriz
	{
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				
		*(int*)buf = g2;			buf += sizeof(int);				
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);				
		*(int*)buf = 0;				buf += sizeof(int);				
	}
	
	*(int*)buf = ess->price;		buf += sizeof(int);				//preço
	*(int*)buf = 0;					buf += sizeof(int);   			//máscara
	
	
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);
	char ** item_content = (char **)buf;	buf += sizeof(char *);
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	*(int*)buf = ess->id;					buf += sizeof(int);   			//item_id
	*(char*)buf = 1;					buf += sizeof(char);   			//despertado

	if(addon_num == 5 && moon_num > 0)
	{
		*(char*)buf = moon_num;					buf += sizeof(char);
	}
	else 
	{
		*(char*)buf = 0;					buf += sizeof(char);
	}

	*(char*)buf = 0;					buf += sizeof(char);   			//lock_unlock

	// addons
	*(int*)buf = addon_num;		buf += sizeof(int);

	std::unordered_map<int, bool> generated_ids; // Não deixa duplicar

	for (int i = 0; i < addon_num; i++)
	{
		addon_data addon_list;
		int r = abase::RandSelect(&ess->addon_list[class_pos].addons[0].probability_addon, sizeof(ess->addon_list[class_pos].addons[0]), 32);
		int addon_id = ess->addon_list[class_pos].addons[r].id_addon;
	
		// Verificação completa de os atributos já foram selecionados
		while (generated_ids.find(addon_id) != generated_ids.end())
		{
			r = abase::RandSelect(&ess->addon_list[class_pos].addons[0].probability_addon, sizeof(ess->addon_list[class_pos].addons[0]), 32);
			addon_id = ess->addon_list[class_pos].addons[r].id_addon;
		}

		generated_ids[addon_id] = true;

		if (!generate_addon(addon_id, addon_list))
		{
			return 0;
		}

		*(int *)buf = addon_list.id;
		buf += sizeof(int);

		bool res = false;
	
		if (!res) 
		{
			*(int *)buf = addon_list.arg[0];
			buf += sizeof(int);
		}
	}

	set_to_classid(DT_QIHUN_ESSENCE, (item_data*)(*data), -1);
	return 0;
}


// Cristal
template <typename RAND_CLASS>
int generate_qiling_item(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls,element_data::GEN_ADDON_MODE normal_addon ,const void * tag,unsigned int tag_size, int class_pos, int * sa_list = NULL)
{
	DATA_TYPE datatype;
	QILING_ESSENCE * ess = (QILING_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_QILING_ESSENCE)	return -1;

	size = sizeof(item_data) + sizeof(_qiling_essence);

	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;					buf += sizeof(unsigned int); //Id
	*(unsigned int*)buf = 1;					buf += sizeof(unsigned int); //número	
	*(unsigned int*)buf = ess->pile_num_max;	buf += sizeof(unsigned int); //número máximo

	*(int*)buf = 0;								buf += sizeof(unsigned int); //octeto
	*(int*)buf = ess->proc_type;				buf += sizeof(unsigned int); //vinculante
	
	*(int*)buf = DT_QILING_ESSENCE;	buf += sizeof(unsigned int); //número da folha	
	
	if(ess->has_guid == 1) // gerar uma diretriz
	{
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				
		*(int*)buf = g2;			buf += sizeof(int);				
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);				
		*(int*)buf = 0;				buf += sizeof(int);				
	}
	
	*(int*)buf = ess->price;		buf += sizeof(int);				//preço
	*(int*)buf = 0;					buf += sizeof(int);   			//máscara
	
	
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);
	char ** item_content = (char **)buf;	buf += sizeof(char *);
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	int crystal_level = 1;

	*(int*)buf = ess->id;					buf += sizeof(int);   			//item_id
	*(char*)buf = crystal_level;					buf += sizeof(char);   			//level

	char generate_color = abase::Rand(0, 4);

	*(char*)buf = generate_color;					buf += sizeof(char);   			//cor
	*(int*)buf = 0;					buf += sizeof(int);   			//exp
	*(char*)buf = 0;					buf += sizeof(char);   			//lock/unlock

	// Estrelas
	for (int i = 0; i < 5; i++)
	{
		if(generate_color == i) 
		{
			*(char*)buf = 1;		buf += sizeof(char);
			continue;
		}
		*(char*)buf = 0;		buf += sizeof(char);	
	}

	// Reserve ??
	for (int i = 0; i < 15; i++)
	{
		*(char*)buf = 0;		buf += sizeof(char);
	}

	// Addons
	int count_addons = 0; // Verifica se há addons na lista
	for (int i = 0; i < 5; i++)
	{
		if(ess->addons[i] != 0) count_addons++;
	}

	*(int*)buf = count_addons;		buf += sizeof(int); // Gera a quantidade de addons
	for (int i = 0; i < count_addons; i++)
	{
		addon_data addon_list;
		unsigned int addon_id = ess->addons[i];

		if (!generate_addon(addon_id, addon_list))
		{
			return 0;
		}

		*(int *)buf = addon_list.id;		buf += sizeof(int);
		*(int *)buf = addon_list.arg[0];		buf += sizeof(int);
	}

	set_to_classid(DT_QILING_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_qihun_cover_item(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	QIHUN_COVER_ESSENCE * ess = (QIHUN_COVER_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_QIHUN_COVER_ESSENCE)	return -1;

	size = sizeof(item_data);
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;					buf += sizeof(unsigned int); //Id
	*(unsigned int*)buf = 1;					buf += sizeof(unsigned int); //número	
	*(unsigned int*)buf = ess->pile_num_max;	buf += sizeof(unsigned int); //número máximo

	*(int*)buf = 0;								buf += sizeof(unsigned int); //octeto
	*(int*)buf = ess->proc_type;				buf += sizeof(unsigned int); //vinculante
	
	*(int*)buf = DT_QIHUN_COVER_ESSENCE;	buf += sizeof(unsigned int); //número da folha	
	
	if(ess->has_guid == 1) // gerar uma diretriz
	{
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				
		*(int*)buf = g2;			buf += sizeof(int);				
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);				
		*(int*)buf = 0;				buf += sizeof(int);				
	}
	
	*(int*)buf = ess->price;		buf += sizeof(int);				//preço
	*(int*)buf = 0;					buf += sizeof(int);   			//máscara
	
	
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	set_to_classid(DT_QIHUN_COVER_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_slide_skill_item(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	SLIDE_SKILL_ESSENCE * ess = (SLIDE_SKILL_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_SLIDE_SKILL_ESSENCE)	return -1;

	size = sizeof(item_data);
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;					buf += sizeof(unsigned int); //Id
	*(unsigned int*)buf = 1;					buf += sizeof(unsigned int); //número	
	*(unsigned int*)buf = ess->pile_num_max;	buf += sizeof(unsigned int); //número máximo

	*(int*)buf = 0;								buf += sizeof(unsigned int); //octeto
	*(int*)buf = ess->proc_type;				buf += sizeof(unsigned int); //vinculante
	
	*(int*)buf = DT_SLIDE_SKILL_ESSENCE;	buf += sizeof(unsigned int); //número da folha	
	
	if(ess->has_guid == 1) // gerar uma diretriz
	{
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				
		*(int*)buf = g2;			buf += sizeof(int);				
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);				
		*(int*)buf = 0;				buf += sizeof(int);				
	}
	
	*(int*)buf = ess->price;		buf += sizeof(int);				//preço
	*(int*)buf = 0;					buf += sizeof(int);   			//máscara
	
	
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	set_to_classid(DT_SLIDE_SKILL_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_use_for_self_item(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	USE_FOR_SELF_ITEM_ESSENCE * ess = (USE_FOR_SELF_ITEM_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_USE_FOR_SELF_ITEM_ESSENCE)	return -1;

	size = sizeof(item_data);
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;					buf += sizeof(unsigned int); //Id
	*(unsigned int*)buf = 1;					buf += sizeof(unsigned int); //número	
	*(unsigned int*)buf = ess->pile_num_max;	buf += sizeof(unsigned int); //número máximo

	*(int*)buf = 0;								buf += sizeof(unsigned int); //octeto
	*(int*)buf = ess->proc_type;				buf += sizeof(unsigned int); //vinculante
	
	*(int*)buf = DT_USE_FOR_SELF_ITEM_ESSENCE;	buf += sizeof(unsigned int); //número da folha	
	
	if(ess->has_guid == 1) // gerar uma diretriz
	{
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				
		*(int*)buf = g2;			buf += sizeof(int);				
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);				
		*(int*)buf = 0;				buf += sizeof(int);				
	}
	
	*(int*)buf = ess->price;		buf += sizeof(int);				//preço
	*(int*)buf = 0;					buf += sizeof(int);   			//máscara
	
	
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	set_to_classid(DT_USE_FOR_SELF_ITEM_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_camp_token_item(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	CAMP_TOKEN_ESSENCE * ess = (CAMP_TOKEN_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_CAMP_TOKEN_ESSENCE)	return -1;

	size = sizeof(item_data) + 	sizeof(prerequisition) + sizeof(size_t) + sizeof(char) * 4;
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);
	
	*(unsigned int *)buf = id;			buf += sizeof(unsigned int);//ģ��id
	*(unsigned int *)buf = 1;					buf += sizeof(unsigned int);		//count
	*(unsigned int *)buf = ess->pile_num_max;	buf += sizeof(unsigned int);
	*(int *)buf = ELEMENTDATAMAN_EQUIP_MASK_FORCE_TICKET;	buf += sizeof(int);		//equip_mask
	*(int *)buf = ess->proc_type;		buf += sizeof(int);		//proc_type
	*(int *)buf = DT_CAMP_TOKEN_ESSENCE;	buf += sizeof(int);		//classid
	
	if(ess->has_guid == 1) // gerar uma diretriz
	{
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				
		*(int*)buf = g2;			buf += sizeof(int);				
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);				
		*(int*)buf = 0;				buf += sizeof(int);				
	}
	
	*(int*)buf = ess->price;		buf += sizeof(int);				//preço
	*(int*)buf = 0;					buf += sizeof(int);   			//máscara
	
	
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	// prerequisition
	char * require_ptr = buf;
	*(short*)buf = ess->require_level;		buf += sizeof(short);				//prerequisition level
	*(short*)buf = ess->character_combo_id&0xFFFF;	buf += sizeof(short);				//prerequisition race
	*(short*)buf = 0;		buf += sizeof(short);				//prerequisition strength
	*(short*)buf = 0;		buf += sizeof(short);				//prerequisition val
	*(short*)buf = 0;		buf += sizeof(short);				//prerequisition agility
	*(short*)buf = 0;		buf += sizeof(short);				//prerequisition energy

	*(int*)buf = 100;						buf += sizeof(int);				//prerequisition durability
	*(int*)buf = 100;						buf += sizeof(int);				//prerequisition max_durability

	// camp token buffer
	*(int*)buf = 6;						buf += sizeof(int);	// size level * 3 + 3 char
	*(char*)buf = 1;						buf += sizeof(char);
	*(int*)buf = 0;						buf += sizeof(int);

	// arrays
	*(char*)buf = 1;						buf += sizeof(char);
	*(char*)buf = 0;						buf += sizeof(char);
	*(char*)buf = 0;						buf += sizeof(char);

	set_to_classid(DT_CAMP_TOKEN_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_camp_token_upgrade_item(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	CAMP_TOKEN_UPGRADE_ESSENCE * ess = (CAMP_TOKEN_UPGRADE_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_CAMP_TOKEN_UPGRADE_ESSENCE)	return -1;

	size = sizeof(item_data);
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;					buf += sizeof(unsigned int); //Id
	*(unsigned int*)buf = 1;					buf += sizeof(unsigned int); //número	
	*(unsigned int*)buf = ess->pile_num_max;	buf += sizeof(unsigned int); //número máximo

	*(int*)buf = 0;								buf += sizeof(unsigned int); //octeto
	*(int*)buf = ess->proc_type;				buf += sizeof(unsigned int); //vinculante
	
	*(int*)buf = DT_CAMP_TOKEN_UPGRADE_ESSENCE;	buf += sizeof(unsigned int); //número da folha	
	
	if(ess->has_guid == 1) // gerar uma diretriz
	{
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				
		*(int*)buf = g2;			buf += sizeof(int);				
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);				
		*(int*)buf = 0;				buf += sizeof(int);				
	}
	
	*(int*)buf = ess->price;		buf += sizeof(int);				//preço
	*(int*)buf = 0;					buf += sizeof(int);   			//máscara
	
	
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	set_to_classid(DT_CAMP_TOKEN_UPGRADE_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_red_book_upgrade_item(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	RED_BOOK_UPGRADE_ITEM * ess = (RED_BOOK_UPGRADE_ITEM *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_RED_BOOK_UPGRADE_ITEM)	return -1;

	size = sizeof(item_data);
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;					buf += sizeof(unsigned int); //Id
	*(unsigned int*)buf = 1;					buf += sizeof(unsigned int); //número	
	*(unsigned int*)buf = ess->pile_num_max;	buf += sizeof(unsigned int); //número máximo

	*(int*)buf = 0;								buf += sizeof(unsigned int); //octeto
	*(int*)buf = ess->proc_type;				buf += sizeof(unsigned int); //vinculante
	
	*(int*)buf = DT_RED_BOOK_UPGRADE_ITEM;	buf += sizeof(unsigned int); //número da folha	
	
	if(ess->has_guid == 1) // gerar uma diretriz
	{
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				
		*(int*)buf = g2;			buf += sizeof(int);				
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);				
		*(int*)buf = 0;				buf += sizeof(int);				
	}
	
	*(int*)buf = ess->price;		buf += sizeof(int);				//preço
	*(int*)buf = 0;					buf += sizeof(int);   			//máscara
	
	
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	set_to_classid(DT_RED_BOOK_UPGRADE_ITEM, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_camp_token_rollback_item(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	CAMP_TOKEN_ROLLBACK_ESSENCE * ess = (CAMP_TOKEN_ROLLBACK_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_CAMP_TOKEN_ROLLBACK_ESSENCE)	return -1;

	size = sizeof(item_data);
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;					buf += sizeof(unsigned int); //Id
	*(unsigned int*)buf = 1;					buf += sizeof(unsigned int); //número	
	*(unsigned int*)buf = ess->pile_num_max;	buf += sizeof(unsigned int); //número máximo

	*(int*)buf = 0;								buf += sizeof(unsigned int); //octeto
	*(int*)buf = ess->proc_type;				buf += sizeof(unsigned int); //vinculante
	
	*(int*)buf = DT_CAMP_TOKEN_ROLLBACK_ESSENCE;	buf += sizeof(unsigned int); //número da folha	
	
	if(ess->has_guid == 1) // gerar uma diretriz
	{
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				
		*(int*)buf = g2;			buf += sizeof(int);				
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);				
		*(int*)buf = 0;				buf += sizeof(int);				
	}
	
	*(int*)buf = ess->price;		buf += sizeof(int);				//preço
	*(int*)buf = 0;					buf += sizeof(int);   			//máscara
	
	
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	set_to_classid(DT_CAMP_TOKEN_ROLLBACK_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_camp_token_prob_adjust_item(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	CAMP_TOKEN_PROB_ADJUST_ESSENCE * ess = (CAMP_TOKEN_PROB_ADJUST_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_CAMP_TOKEN_PROB_ADJUST_ESSENCE)	return -1;

	size = sizeof(item_data);
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;					buf += sizeof(unsigned int); //Id
	*(unsigned int*)buf = 1;					buf += sizeof(unsigned int); //número	
	*(unsigned int*)buf = ess->pile_num_max;	buf += sizeof(unsigned int); //número máximo

	*(int*)buf = 0;								buf += sizeof(unsigned int); //octeto
	*(int*)buf = ess->proc_type;				buf += sizeof(unsigned int); //vinculante
	
	*(int*)buf = DT_CAMP_TOKEN_PROB_ADJUST_ESSENCE;	buf += sizeof(unsigned int); //número da folha	
	
	if(ess->has_guid == 1) // gerar uma diretriz
	{
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				
		*(int*)buf = g2;			buf += sizeof(int);				
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);				
		*(int*)buf = 0;				buf += sizeof(int);				
	}
	
	*(int*)buf = ess->price;		buf += sizeof(int);				//preço
	*(int*)buf = 0;					buf += sizeof(int);   			//máscara
	
	
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	set_to_classid(DT_CAMP_TOKEN_PROB_ADJUST_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_illustrated_fashion_item(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	ILLUSTRATED_FASHION_ESSENCE * ess = (ILLUSTRATED_FASHION_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_ILLUSTRATED_FASHION_ESSENCE)	return -1;

	size = sizeof(item_data);
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;					buf += sizeof(unsigned int); //Id
	*(unsigned int*)buf = 1;					buf += sizeof(unsigned int); //número	
	*(unsigned int*)buf = ess->pile_num_max;	buf += sizeof(unsigned int); //número máximo

	*(int*)buf = 0;								buf += sizeof(unsigned int); //octeto
	*(int*)buf = ess->proc_type;				buf += sizeof(unsigned int); //vinculante
	
	*(int*)buf = DT_ILLUSTRATED_FASHION_ESSENCE;	buf += sizeof(unsigned int); //número da folha	
	
	if(ess->has_guid == 1) // gerar uma diretriz
	{
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				
		*(int*)buf = g2;			buf += sizeof(int);				
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);				
		*(int*)buf = 0;				buf += sizeof(int);				
	}
	
	*(int*)buf = ess->price;		buf += sizeof(int);				//preço
	*(int*)buf = 0;					buf += sizeof(int);   			//máscara
	
	
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	set_to_classid(DT_ILLUSTRATED_FASHION_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_illustrated_wing_egg_item(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	ILLUSTRATED_WING_EGG_ESSENCE * ess = (ILLUSTRATED_WING_EGG_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_ILLUSTRATED_WING_EGG_ESSENCE)	return -1;

	size = sizeof(item_data);
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;					buf += sizeof(unsigned int); //Id
	*(unsigned int*)buf = 1;					buf += sizeof(unsigned int); //número	
	*(unsigned int*)buf = ess->pile_num_max;	buf += sizeof(unsigned int); //número máximo

	*(int*)buf = 0;								buf += sizeof(unsigned int); //octeto
	*(int*)buf = ess->proc_type;				buf += sizeof(unsigned int); //vinculante
	
	*(int*)buf = DT_ILLUSTRATED_WING_EGG_ESSENCE;	buf += sizeof(unsigned int); //número da folha	
	
	if(ess->has_guid == 1) // gerar uma diretriz
	{
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				
		*(int*)buf = g2;			buf += sizeof(int);				
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);				
		*(int*)buf = 0;				buf += sizeof(int);				
	}
	
	*(int*)buf = ess->price;		buf += sizeof(int);				//preço
	*(int*)buf = 0;					buf += sizeof(int);   			//máscara
	
	
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	set_to_classid(DT_ILLUSTRATED_WING_EGG_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_illustrated_pet_egg_item(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	ILLUSTRATED_PET_EGG_ESSENCE * ess = (ILLUSTRATED_PET_EGG_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_ILLUSTRATED_PET_EGG_ESSENCE)	return -1;

	size = sizeof(item_data);
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;					buf += sizeof(unsigned int); //Id
	*(unsigned int*)buf = 1;					buf += sizeof(unsigned int); //número	
	*(unsigned int*)buf = ess->pile_num_max;	buf += sizeof(unsigned int); //número máximo

	*(int*)buf = 0;								buf += sizeof(unsigned int); //octeto
	*(int*)buf = ess->proc_type;				buf += sizeof(unsigned int); //vinculante
	
	*(int*)buf = DT_ILLUSTRATED_PET_EGG_ESSENCE;	buf += sizeof(unsigned int); //número da folha	
	
	if(ess->has_guid == 1) // gerar uma diretriz
	{
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				
		*(int*)buf = g2;			buf += sizeof(int);				
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);				
		*(int*)buf = 0;				buf += sizeof(int);				
	}
	
	*(int*)buf = ess->price;		buf += sizeof(int);				//preço
	*(int*)buf = 0;					buf += sizeof(int);   			//máscara
	
	
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	set_to_classid(DT_ILLUSTRATED_PET_EGG_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_fast_produce_item(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	FAST_PRODUCE_ITEM_ESSENCE * ess = (FAST_PRODUCE_ITEM_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_FAST_PRODUCE_ITEM_ESSENCE)	return -1;

	size = sizeof(item_data);
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;					buf += sizeof(unsigned int); //Id
	*(unsigned int*)buf = 1;					buf += sizeof(unsigned int); //número	
	*(unsigned int*)buf = ess->pile_num_max;	buf += sizeof(unsigned int); //número máximo

	*(int*)buf = 0;								buf += sizeof(unsigned int); //octeto
	*(int*)buf = ess->proc_type;				buf += sizeof(unsigned int); //vinculante
	
	*(int*)buf = DT_FAST_PRODUCE_ITEM_ESSENCE;	buf += sizeof(unsigned int); //número da folha	
	
	if(ess->has_guid == 1) // gerar uma diretriz
	{
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				
		*(int*)buf = g2;			buf += sizeof(int);				
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);				
		*(int*)buf = 0;				buf += sizeof(int);				
	}
	
	*(int*)buf = ess->price;		buf += sizeof(int);				//preço
	*(int*)buf = 0;					buf += sizeof(int);   			//máscara
	
	
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	set_to_classid(DT_FAST_PRODUCE_ITEM_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_kid_debris_item(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	KID_DEBRIS_ESSENCE * ess = (KID_DEBRIS_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_KID_DEBRIS_ESSENCE)	return -1;

	size = sizeof(item_data);
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;					buf += sizeof(unsigned int); //Id
	*(unsigned int*)buf = 1;					buf += sizeof(unsigned int); //número	
	*(unsigned int*)buf = ess->pile_num_max;	buf += sizeof(unsigned int); //número máximo

	*(int*)buf = 0;								buf += sizeof(unsigned int); //octeto
	*(int*)buf = ess->proc_type;				buf += sizeof(unsigned int); //vinculante
	
	*(int*)buf = DT_KID_DEBRIS_ESSENCE;	buf += sizeof(unsigned int); //número da folha	
	
	if(ess->has_guid == 1) // gerar uma diretriz
	{
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				
		*(int*)buf = g2;			buf += sizeof(int);				
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);				
		*(int*)buf = 0;				buf += sizeof(int);				
	}
	
	*(int*)buf = ess->price;		buf += sizeof(int);				//preço
	*(int*)buf = 0;					buf += sizeof(int);   			//máscara
	
	
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	set_to_classid(DT_KID_DEBRIS_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_kid_debris_generator_item(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	KID_DEBRIS_GENERATOR_ESSENCE * ess = (KID_DEBRIS_GENERATOR_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_KID_DEBRIS_GENERATOR_ESSENCE)	return -1;

	size = sizeof(item_data);
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;					buf += sizeof(unsigned int); //Id
	*(unsigned int*)buf = 1;					buf += sizeof(unsigned int); //número	
	*(unsigned int*)buf = ess->pile_num_max;	buf += sizeof(unsigned int); //número máximo

	*(int*)buf = 0;								buf += sizeof(unsigned int); //octeto
	*(int*)buf = ess->proc_type;				buf += sizeof(unsigned int); //vinculante
	
	*(int*)buf = DT_KID_DEBRIS_GENERATOR_ESSENCE;	buf += sizeof(unsigned int); //número da folha	
	
	if(ess->has_guid == 1) // gerar uma diretriz
	{
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				
		*(int*)buf = g2;			buf += sizeof(int);				
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);				
		*(int*)buf = 0;				buf += sizeof(int);				
	}
	
	*(int*)buf = ess->price;		buf += sizeof(int);				//preço
	*(int*)buf = 0;					buf += sizeof(int);   			//máscara
	
	
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	set_to_classid(DT_KID_DEBRIS_GENERATOR_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_material_refine_item(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	MATERIAL_REFINE_ESSENCE * ess = (MATERIAL_REFINE_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_MATERIAL_REFINE_ESSENCE)	return -1;

	size = sizeof(item_data);
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;					buf += sizeof(unsigned int); //Id
	*(unsigned int*)buf = 1;					buf += sizeof(unsigned int); //número	
	*(unsigned int*)buf = ess->pile_num_max;	buf += sizeof(unsigned int); //número máximo

	*(int*)buf = 0;								buf += sizeof(unsigned int); //octeto
	*(int*)buf = ess->proc_type;				buf += sizeof(unsigned int); //vinculante
	
	*(int*)buf = DT_MATERIAL_REFINE_ESSENCE;	buf += sizeof(unsigned int); //número da folha	
	
	if(ess->has_guid == 1) // gerar uma diretriz
	{
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				
		*(int*)buf = g2;			buf += sizeof(int);				
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);				
		*(int*)buf = 0;				buf += sizeof(int);				
	}
	
	*(int*)buf = ess->price;		buf += sizeof(int);				//preço
	*(int*)buf = 0;					buf += sizeof(int);   			//máscara
	
	
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	set_to_classid(DT_MATERIAL_REFINE_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

template <typename RAND_CLASS>
int generate_material_refine_ticket_item(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	MATERIAL_REFINE_TICKET_ESSENCE * ess = (MATERIAL_REFINE_TICKET_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_MATERIAL_REFINE_TICKET_ESSENCE)	return -1;

	size = sizeof(item_data);
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);

	*(unsigned int*)buf = id;					buf += sizeof(unsigned int); //Id
	*(unsigned int*)buf = 1;					buf += sizeof(unsigned int); //número	
	*(unsigned int*)buf = ess->pile_num_max;	buf += sizeof(unsigned int); //número máximo

	*(int*)buf = 0;								buf += sizeof(unsigned int); //octeto
	*(int*)buf = ess->proc_type;				buf += sizeof(unsigned int); //vinculante
	
	*(int*)buf = DT_MATERIAL_REFINE_TICKET_ESSENCE;	buf += sizeof(unsigned int); //número da folha	
	
	if(ess->has_guid == 1) // gerar uma diretriz
	{
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				
		*(int*)buf = g2;			buf += sizeof(int);				
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);				
		*(int*)buf = 0;				buf += sizeof(int);				
	}
	
	*(int*)buf = ess->price;		buf += sizeof(int);				//preço
	*(int*)buf = 0;					buf += sizeof(int);   			//máscara
	
	
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	set_to_classid(DT_MATERIAL_REFINE_TICKET_ESSENCE, (item_data*)(*data), -1);
	return 0;
}
//------------------------------------------------------------------------------------------------------------------------	
	
template <typename RAND_CLASS>
int generate_rune_slot_item_essence(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	UNLOCK_RUNE_SLOT_ITEM_ESSENCE * ess = (UNLOCK_RUNE_SLOT_ITEM_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_UNLOCK_RUNE_SLOT_ITEM_ESSENCE)	return -1;

	size = sizeof(item_data);

	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);
	
	*(unsigned int*)buf = id;					buf += sizeof(unsigned int); //ид
	*(unsigned int*)buf = 1;					buf += sizeof(unsigned int); //количество	
	*(unsigned int*)buf = ess->pile_num_max;	buf += sizeof(unsigned int); //макс количество

	*(int*)buf = 0;								buf += sizeof(int);				//октет
	*(int*)buf = ess->proc_type;				buf += sizeof(int);				//привязка
	
	*(unsigned int*)buf = DT_UNLOCK_RUNE_SLOT_ITEM_ESSENCE;	buf += sizeof(unsigned int); //номер листа	
	
	if(ess->has_guid == 1) // генерировать гуид
	{
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				
		*(int*)buf = g2;			buf += sizeof(int);				
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);				
		*(int*)buf = 0;				buf += sizeof(int);				
	}
	
	*(int*)buf = ess->price;				buf += sizeof(int);				//цена
	*(int*)buf = 0;	buf += sizeof(int);   //маска
	
	
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;

	set_to_classid(DT_UNLOCK_RUNE_SLOT_ITEM_ESSENCE, (item_data*)(*data), -1);
	return 0;
}


template <typename RAND_CLASS>
int generate_rune_item_essence(unsigned int id, ID_SPACE idspace, char ** data, unsigned int& size, RAND_CLASS cls)
{
	DATA_TYPE datatype;
	RUNE_ITEM_ESSENCE * ess = (RUNE_ITEM_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if(ess == NULL || datatype != DT_RUNE_ITEM_ESSENCE)	return -1;

	size = sizeof(item_data);
	
	// ���addons
	char addon_buf[ELEMENTDATAMAN_MAX_NUM_ADDONS*sizeof(_addon)];
	unsigned int addon_num = 20;
	unsigned int addon_size = 0;
	addon_size = generate_equipment_addon_buffer_2(DT_RUNE_ITEM_ESSENCE, (int*)ess->addon, sizeof(int),addon_num, addon_buf,addon_num);
	size += sizeof(int);
	size += addon_size;

	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = (*data);
	
	*(unsigned int*)buf = id;					buf += sizeof(unsigned int); //ид
	*(unsigned int*)buf = 1;					buf += sizeof(unsigned int); //количество	
	*(unsigned int*)buf = ess->pile_num_max;	buf += sizeof(unsigned int); //макс количество

	*(int*)buf = 0;								buf += sizeof(int);				//октет
	*(int*)buf = ess->proc_type;				buf += sizeof(int);				//привязка
	
	*(unsigned int*)buf = DT_RUNE_ITEM_ESSENCE;	buf += sizeof(unsigned int); //номер листа	
	
	if(ess->has_guid == 1) // генерировать гуид
	{
		int g1,g2;
		get_item_guid(id,g1,g2);
		*(int*)buf = g1;			buf += sizeof(int);				
		*(int*)buf = g2;			buf += sizeof(int);				
	}
	else{
		*(int*)buf = 0;				buf += sizeof(int);				
		*(int*)buf = 0;				buf += sizeof(int);				
	}
	
	*(int*)buf = ess->price;				buf += sizeof(int);				//цена
	*(int*)buf = 0;	buf += sizeof(int);   //маска
	
	
	unsigned int* content_length = (unsigned int*)buf;	buf += sizeof(unsigned int);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;	buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char*)(*data)+size-buf;
	*item_content = buf;
	
	
	// addons
	*(int*)buf = addon_num;				buf += sizeof(int);
	char * addon_sld = addon_buf;
	for(unsigned int i=0; i<addon_num; i++)
	{
		int id = *(int*)addon_sld;
		addon_sld += ((id & 0x6000)>>13)*sizeof(int)+sizeof(int);
	}
	memcpy(buf, addon_buf, addon_size);	
	buf += addon_size;
	ASSERT(buf - *data == (int)size);
	

	set_to_classid(DT_RUNE_ITEM_ESSENCE, (item_data*)(*data), -1);
	return 0;
}

