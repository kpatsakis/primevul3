person_set_pose(person_t* person, const char* pose_name)
{
	person->direction = realloc(person->direction, (strlen(pose_name) + 1) * sizeof(char));
	strcpy(person->direction, pose_name);
}
