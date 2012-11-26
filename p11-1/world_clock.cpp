  #include "world_clock.h"
map<string, int> WorldClock :: timezone_;
  WorldClock::WorldClock() : time(0),time_difference_(0)
  {
	}
  WorldClock::WorldClock(int hour, int minute, int second) :time_difference_(0)
  {
		time = hour*3600+minute*60+second; // convert to second 
	}
  void WorldClock:: Tick(int seconds)
  {
		time+=seconds;
		if (time < 0) // when seconds is negative, time can be negative
			time=time%(3600*24)+(3600*24); // so plus one day
	}
  // �߸��� �� �Է½� false �����ϰ� ���� �ð��� �ٲ��� ����.
  bool WorldClock::SetTime(int hour, int minute, int second)
  {
		if (hour < 0 || minute < 0 || second < 0 || hour >= 24 || minute >= 60 || second >= 60) // if out of range
			return 0;
		time = hour*3600+minute*60+second; // convert to second
		return 1;
	}
  //static bool LoadTimezoneFromFile(const string& file_path); 
  //ä������ ������ ���� ������� �������
  //�ʰ� �Ʒ��� �Լ�(AddTimezoneInfo)�� ����մϴ�.��
 void WorldClock::AddTimezoneInfo(const string& city, int diff)
  {
		timezone_[city] = diff;
	}
  // �߸��� �� �Է½� false �����ϰ� ���� �ð��� �ٲ��� ����.
  bool WorldClock::SetTimezone(const string& timezone)
  {
		if (timezone_.find(timezone) == timezone_.end()) // can't find
			return 0;
		time_difference_ = timezone_[timezone];
		return 1;
	}
  int WorldClock::hour() const
  {
		return (time/3600)%24;
	}
  int WorldClock::minute() const
  {
		return (time%3600)/60;
	}
  int WorldClock::second() const
  {
		return time%60;
	}
// hh:mm:ss �������� �����. ǥ�ؽð� �ƴ� ��� (+xx) �������� ������ ǥ��.
// ����� �Է� ������ >> operator�� InvalidDateException�� �߻��� �� ����.
ostream& operator<<(ostream& os, const WorldClock& c)
{
	WorldClock temp = c;
	temp.Tick(temp.time_difference()*3600); // plus difference
	os << temp.hour() << ":" << temp.minute() << ":" << temp.second();
	if (temp.time_difference() > 0) // if not GMT -> difference != 0
		os << " (+" << temp.time_difference() << ")";
	if (temp.time_difference() < 0)
		os << " (" << temp.time_difference() << ")";
	return os;
}
istream& operator>>(istream& is, WorldClock& c)  // hh:mm:ss �� �Է¹���.
{
	string temp,temp2;
	int i,last=0,num[10],num_cnt=0;
	is >> temp;
	for (i=0;i<temp.length();++i)
	{
		if (temp[i] == ':')
		{
			temp2 = temp.substr(last,i-last);
			num[num_cnt++] = atoi(temp2.c_str());
			last = i+1;
		}
	}
	temp2 = temp.substr(last,i-last);
	num[num_cnt++] = atoi(temp2.c_str());
	last = i+1;	
	if (c.SetTime(num[0],num[1],num[2]) == 0)
		throw InvalidTimeException(temp);
	return is;
}
