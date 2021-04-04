#pragma once
#include "MySerializer.h"

enum class MyPacketType : uint16_t {
	None,
	Hello,
	Chat,
};

class MyPacket : public MyNonCopyable {
public:
	using Type = MyPacketType;
	using PacketSize = uint32_t;

	static const size_t kMaxPacketSize = 32 * 1024;

	void toBuffer(std::vector<char> & buf) {
		buf.clear();
		MySerializer se(buf);
		onToBuffer(se);

		PacketSize packetSize = 0;
		{
			auto s = se.buf().size();
			if (s > kMaxPacketSize)
				throw MyError("excess MaxPacketSize");
			packetSize = static_cast<PacketSize>(s);
		}
		
		{
			// update the header size in buf
			auto* p = reinterpret_cast<PacketSize*>(se.buf().data());
			*p = my_hton(packetSize);
		}
	}

	void fromBuffer(const std::vector<char> & buf) {
		MyDeserializer de(buf);
		onFromBuffer(de);
	}

	Type type() const { return onType(); }

protected:
	virtual Type onType() const = 0;
	virtual void onToBuffer  (MySerializer&   se) = 0;
	virtual void onFromBuffer(MyDeserializer& se) = 0;

	void io(MySerializer&   se) {
		PacketSize size = 0;
		se.io_fixed(size);

		Type t = type();
		se.io_fixed(my_enum_to_int(t));
	}

	void io(MyDeserializer& se) {
		PacketSize	size = 0;
		se.io_fixed(size);

		Type		t = Type::None;
		se.io_fixed(my_enum_to_int(t));
		if (t != type())
			throw MyError("Packet type mismatch");
	}
};

class MyPacket_Hello : public MyPacket {
	using Base = MyPacket;
public:
	uint32_t version = 100;

protected:
	virtual Type onType() const { return Type::Hello; }
	virtual void onToBuffer  (MySerializer&   se) override { io(se); }
	virtual void onFromBuffer(MyDeserializer& se) override { io(se); }

	template<typename SE>
	void io(SE& se) {
		Base::io(se);
		se.io(version);
	}
};

class MyPacket_Chat : public MyPacket {
	using Base = MyPacket;
public:
	std::string msg;
	std::vector< std::string > toUsers;

protected:
	virtual Type onType() const { return Type::Chat; }
	virtual void onToBuffer  (MySerializer&   se) override { io(se); }
	virtual void onFromBuffer(MyDeserializer& se) override { io(se); }

	template<typename SE>
	void io(SE& se) {
		Base::io(se);
		se.io(msg);
		se.io(toUsers);
	}
};
