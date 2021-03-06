// ----------------------------------------------------------------------------
//
//     Filename   : json_value.h
//
//     Author     : Benny Bach <benny.bach@gmail.com>
//                  Copyright (C) 2014
//
// --- Description: -----------------------------------------------------------
//
//
// ----------------------------------------------------------------------------
#ifndef __json__value_h__
#define __json__value_h__

// ----------------------------------------------------------------------------
#include <string>
#include <memory>
#include <cassert>
#include <sstream>

// ----------------------------------------------------------------------------
namespace json
{
  enum class type
  {
    nul = 0x00,
    num = 0x01,
    tru = 0x02,
    fal = 0x03,
    str = 0x81,
    obj = 0x82,
    arr = 0x83
  };

  class object;
  class array;

  class value
  {
    using str_ptr = std::string*;
    using arr_ptr = array*;
    using obj_ptr = object*;
  public:
    value() : type_(type::nul) {}
  public:
    value(const value& other);
    value(value&& other);
    value& operator= (const value& rhs);
    value& operator= (value&& rhs);
  public:
    value(const char* v);
    value(const std::string& v);
    value(std::string&& v);
    value(double v);
    value(int v);
    value(unsigned v);
    value(bool v);
    value(const json::array& v);
    value(json::array&& v);
    value(const json::object& v);
    value(json::object&& v);
  public:
    ~value();
  public:
    operator std::string() const;
    operator double() const;
    operator bool() const;
    operator json::object() const;
    operator json::array() const;
  public:
    type type_id() const noexcept { return type_; }
  public:
    bool is_null()   const noexcept { return type_ == type::nul; }
    bool is_true()   const noexcept { return type_ == type::tru; }
    bool is_false()  const noexcept { return type_ == type::fal; }
    bool is_bool()   const noexcept { return is_true() || is_false(); }
    bool is_string() const noexcept { return type_ == type::str; }
    bool is_number() const noexcept { return type_ == type::num; }
    bool is_array()  const noexcept { return type_ == type::arr; }
    bool is_object() const noexcept { return type_ == type::obj; }
  public:
    const std::string& as_string() const
    {
      assert(is_string());
      return *str_;
    }
  public:
    double as_number() const
    {
      assert(is_number());
      return num_;
    }
  public:
    bool as_bool() const
    {
      assert(is_bool());
      return t_f_;
    }
  public:
    //const array& as_array()  const
    array& as_array() const
    {
      assert(is_array());
      return *arr_;
    }
  public:
    //const object& as_object() const
    object& as_object() const
    {
      assert(is_object());
      return *obj_;
    }
  public:
    std::string move_string()
    {
      assert(is_string());
      return std::move(*str_);
    }
  public:
    void write(std::ostream& os) const;
  private:
    void free_value();
  private:
    type type_;
  private:
    union
    {
      str_ptr str_;
      double  num_;
      bool    t_f_;
      arr_ptr arr_;
      obj_ptr obj_;
    };
  };

  inline std::string escape(const std::string& s)
  {
      std::string result;

      result.reserve(s.size()+2);

      result += "\"";
      for ( auto c : s )
      {
        switch ( c )
        {
          case '"':  result += "\\\""; break;
          case '\\': result += "\\\\"; break;
          case '/':  result += "\\/";  break;
          case '\b': result += "\\b";  break;
          case '\f': result += "\\f";  break;
          case '\n': result += "\\n";  break;
          case '\r': result += "\\r";  break;
          case '\t': result += "\\t";  break;
          default:
            result += c;
        }
      }
      result += "\"";

      return result;
  }
}

// ----------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, json::type t);

// ----------------------------------------------------------------------------
inline std::ostream& operator<<(std::ostream& os, const json::value& v)
{
  v.write(os);
  return os;
}

// ----------------------------------------------------------------------------
inline std::string to_string(const json::value& value)
{
  std::stringstream os;
  os << value;
  return os.str();
}

// ----------------------------------------------------------------------------
#endif // __json__value_h__
